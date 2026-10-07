import subprocess
import json
import os
from pathlib import Path
from .macho import MachO
from .compiler import command, fingerprint, permitted, language_flags
from .report import native_report, objdiff_adapter
from .util import load_json, write_json, ToolError, sha256, local_path, depfile_paths


def configuration(root):
    config = load_json(Path(root) / 'build/config.json')
    if sha256(Path(root) / config['provenance']['input']) != config['provenance']['executable_sha256']:
        raise ToolError('Imported original executable hash mismatch; reimport the verified IPA')
    if sha256(Path(root) / 'build/inventory.json') != config['inventory_sha256']:
        raise ToolError('Generated inventory changed without configuring')
    return config


def compile_flags(config, unit, normalized=False):
    flags = language_flags(unit['source']) + config['compiler']['flags'] + unit['flags']
    if config.get('sdk'):
        flags += ['-isysroot', config['compiler'].get('sdk_compile_path', config['sdk'])]
        if normalized:
            flags = [x.replace(config['sdk'], '<sdk>') for x in flags]
    return flags


def compile_unit(root, uid):
    root = Path(root).resolve()
    config = configuration(root)
    unit = next((u for u in config['units'] if u['id'] == uid), None)
    if unit is None:
        raise ToolError('Unknown unit: ' + uid)
    out = root / 'build/units'
    out.mkdir(parents=True, exist_ok=True)
    obj = out / (uid + '.o')
    dep = out / (uid + '.d')
    old_dep = dep.read_text() if dep.exists() else ''
    # Ninja consumes .d. Keep a copy for compile failures and the watcher.
    saved_dep = out / (uid + '.deps')
    if not old_dep and saved_dep.exists():
        old_dep = saved_dep.read_text()
    info = fingerprint(config['compiler'], root, config.get('sdk'))
    current = {**config, 'compiler_fingerprint': info}
    allowed, error = permitted(current)
    flags = compile_flags(config, unit)
    result = {'id': uid, 'source': unit['source'], 'flags': flags, 'compiler': info,
              'status': 'compile_error', 'object': f'build/units/{uid}.o', 'object_sha256': None,
              'source_sha256': None, 'dependencies': {}, 'diagnostics': f'build/units/{uid}.diagnostics.txt'}
    diagnostics = error or ''
    code = 1
    try:
        source = local_path(root, unit['source'])
        result['source_sha256'] = sha256(source)
        if config['compiler'].get('sdk_required') and (not config.get('sdk') or not Path(config['sdk']).is_dir()):
            allowed = False
            diagnostics += '\nSupply a local iPhoneOS5.1.sdk with configure.py --sdk <path>'
        if allowed:
            cmd = command(config['compiler'], root, config.get('sdk')) + flags + ['-MMD', '-MP', '-MF', str(dep.relative_to(root)),
                  '-MT', f'build/units/{uid}.compile.json', '-c', unit['source'], '-o', str(obj.relative_to(root))]
            env = {**os.environ, 'LC_ALL': 'C', 'TZ': 'UTC', 'SOURCE_DATE_EPOCH': '0'}
            process = subprocess.run(cmd, cwd=root, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                                     text=True, errors='replace', timeout=300)
            diagnostics, code = process.stdout, process.returncode
            if code == 0:
                parsed = MachO(obj.read_bytes())
                if parsed.filetype != 1:
                    raise ToolError('Compiler did not produce an ARMv7 Mach-O object')
                for dependency in depfile_paths(dep):
                    path = Path(dependency)
                    path = path if path.is_absolute() else root / path
                    if path.is_file() and path.resolve().is_relative_to(root):
                        result['dependencies'][str(path.resolve().relative_to(root))] = sha256(path)
                result.update(status='compiled', object_sha256=sha256(obj))
        result['returncode'] = code
    except (OSError, subprocess.TimeoutExpired, ToolError) as e:
        diagnostics += '\n' + str(e)
        result['returncode'] = 1
    if result['status'] != 'compiled':
        obj.unlink(missing_ok=True)  # Never compare a stale successful object.
        if not dep.exists():
            dep.write_text(old_dep or f'build/units/{uid}.compile.json: {unit["source"]}\n')
    if dep.exists():
        saved_dep.write_text(dep.read_text())
    diagnostics = diagnostics.replace(str(root), '<workspace>')
    if config.get('sdk'):
        diagnostics = diagnostics.replace(config['sdk'], '<sdk>')
        result['flags'] = [x.replace(config['sdk'], '<sdk>') for x in flags]
    (out / (uid + '.diagnostics.txt')).write_text(diagnostics)
    write_json(out / (uid + '.compile.json'), result)
    print(('Compiled ' if result['status'] == 'compiled' else 'Compilation failed: ') + unit['source'])
    # Returning success lets Ninja build the report before report() fails CI.
    return result


def comparisons(root, details_id=None):
    from .compare import compare_function
    root = Path(root)
    config = configuration(root)
    inventory = load_json(root / 'build/inventory.json')
    original = MachO((root / config['provenance']['input']).read_bytes())
    info = fingerprint(config['compiler'], root, config.get('sdk'))
    allowed, gate_reason = permitted({**config, 'compiler_fingerprint': info})
    unit_configs = {u['id']: u for u in config['units']}
    objects, compile_results, unit_errors = {}, {}, {}
    for uid, unit in unit_configs.items():
        try:
            result = load_json(root / f'build/units/{uid}.compile.json')
            compile_results[uid] = result
            if result['status'] != 'compiled':
                raise ToolError('Candidate compilation failed; see unit diagnostics')
            if result['compiler'] != info:
                raise ToolError('Compiler fingerprint changed; reconfigure and rebuild')
            if result['flags'] != compile_flags(config, unit, normalized=True) or result['source'] != unit['source']:
                raise ToolError('Candidate compile configuration changed; rebuild')
            if result['source_sha256'] != sha256(local_path(root, unit['source'])):
                raise ToolError('Candidate source changed since compilation; rebuild')
            for dependency, digest in result.get('dependencies', {}).items():
                if sha256(local_path(root, dependency)) != digest:
                    raise ToolError('Included dependency changed since compilation; rebuild: ' + dependency)
            objpath = root / result['object']
            if sha256(objpath) != result['object_sha256']:
                raise ToolError('Candidate object fingerprint changed; rebuild')
            objects[uid] = MachO(objpath.read_bytes())
        except (OSError, ToolError, ValueError) as e:
            unit_errors[uid] = str(e)
    results = []
    for f in inventory['functions']:
        if details_id and f['id'] != details_id:
            continue
        uid = f['group_id']
        u = unit_configs.get(uid, {})
        compared = compare_function(original, inventory, f, objects.get(uid), u.get('functions', {}).get(f['id']),
                                    u.get('placements'), bool(details_id))
        if uid in unit_errors:
            compiled = compile_results.get(uid, {})
            status = 'compile_error' if compiled.get('status') != 'compiled' else 'unresolved'
            compared.update(status=status, reasons=compared['reasons'] + [unit_errors[uid]])
        elif not allowed and compared['status'] != 'missing':
            compared.update(status='unresolved', reasons=compared['reasons'] + [gate_reason])
        compared.update({k: f[k] for k in ('group_id', 'symbol', 'address', 'size', 'mode', 'source_path', 'section', 'ambiguities')})
        compared['section_offset'] = f['address'] - original.section(f['section']).address
        compared['candidate_source'] = u.get('source')
        compared['byte_verified'] = compared['status'] == 'matched'
        results.append(compared)
    if details_id:
        if not results:
            raise ToolError('Unknown function ID: ' + details_id)
        result = results[0]
        result['compiler'] = info
        if result['group_id'] in compile_results:
            result['compile'] = compile_results[result['group_id']]
            result['diagnostic_text'] = (root / result['compile']['diagnostics']).read_text()
        return result
    units = []
    for g in inventory['groups']:
        u = unit_configs.get(g['id'], {})
        units.append({**g, 'source': u.get('source'), 'flags': u.get('flags', []),
                      'compile': compile_results.get(g['id']), 'error': unit_errors.get(g['id'])})
    return native_report(inventory, config, results, units, info)


def report(root):
    result = comparisons(root)
    write_json(Path(root) / 'build/report.json', result)
    write_json(Path(root) / 'build/objdiff-report.json', objdiff_adapter(result))
    m = result['metrics']
    print(f'{m["matched_functions"]}/{m["total_functions"]} verified functions; {m["matched_bytes"]} matched bytes; '
          f'{m["missing_candidates"]} missing; {m["unresolved_comparisons"]} unresolved; {m["compile_errors"]} compile errors')
    return 1 if m['compile_errors'] or any(u['error'] for u in result['units']) else 0
