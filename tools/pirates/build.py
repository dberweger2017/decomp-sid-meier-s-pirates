import subprocess
import json
import os
from pathlib import Path
from .macho import MachO
from .sdk import inspect_sdk, header_flags
from .compiler import command, fingerprint, permitted, language_flags
from .source_policy import SOURCE_POLICY, validate_source
from .report import native_report, objdiff_adapter
from .util import load_json, write_json, ToolError, sha256, local_path, depfile_paths


def configuration(root):
    config = load_json(Path(root) / 'build/config.json')
    if sha256(Path(root) / config['provenance']['input']) != config['provenance']['executable_sha256']:
        raise ToolError('Imported original executable hash mismatch; reimport the verified IPA')
    if sha256(Path(root) / 'build/inventory.json') != config['inventory_sha256']:
        raise ToolError('Generated inventory changed without configuring')
    if config.get('data_inventory_sha256') and sha256(Path(root) / 'build/data-inventory.json') != config['data_inventory_sha256']:
        raise ToolError('Generated data inventory changed without configuring')
    return config


def compile_flags(config, unit, normalized=False):
    flags = language_flags(unit['source']) + config['compiler']['flags'] + unit['flags']
    if config.get('sdk'):
        flags += header_flags(config['compiler'], config['sdk'], unit['source'])
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
    allowed, error = permitted(current, unit['source'])
    flags = compile_flags(config, unit)
    result = {'id': uid, 'source': unit['source'], 'flags': flags, 'compiler': info,
              'sdk_required': bool(unit.get('sdk_required', config['compiler'].get('sdk_required'))),
              'status': 'compile_error', 'object': f'build/units/{uid}.o', 'object_sha256': None,
              'source_policy': {'version': SOURCE_POLICY['version'], 'validated': False},
              'source_sha256': None, 'dependencies': {}, 'diagnostics': f'build/units/{uid}.diagnostics.txt'}
    sdk = inspect_sdk(config.get('sdk'))
    result['sdk'] = sdk
    if sdk and not sdk['validated']:
        allowed, error = False, sdk['reason']
    diagnostics = error or ''
    code = 1
    try:
        source = local_path(root, unit['source'])
        result['source_sha256'] = sha256(source)
        if unit.get('sdk_required', config['compiler'].get('sdk_required')) and (not config.get('sdk') or not Path(config['sdk']).is_dir()):
            allowed = False
            diagnostics += '\nSupply a local iPhoneOS5.1.sdk with configure.py --sdk <path>'
        if allowed:
            driver = command(config['compiler'], root, config.get('sdk')) + flags
            dependencies = ['-MMD', '-MP', '-MF', str(dep.relative_to(root)),
                            '-MT', f'build/units/{uid}.compile.json']
            env = {**os.environ, 'LC_ALL': 'C', 'TZ': 'UTC', 'SOURCE_DATE_EPOCH': '0'}
            preprocessed = subprocess.run(driver + dependencies + ['-E', unit['source']], cwd=root, env=env,
                                          capture_output=True, text=True, errors='replace', timeout=300)
            diagnostics, code = preprocessed.stderr, preprocessed.returncode
            if code:
                raise ToolError('Candidate preprocessing failed; source policy could not be checked')
            validate_source(preprocessed.stdout)
            result['source_policy'] = dict(SOURCE_POLICY)
            cmd = driver + dependencies + ['-c', unit['source'], '-o', str(obj.relative_to(root))]
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


def comparisons(root, details_id=None, include_link=True, details=False):
    from .compare import compare_function
    root = Path(root)
    config = configuration(root)
    inventory = load_json(root / 'build/inventory.json')
    original = MachO((root / config['provenance']['input']).read_bytes())
    info = fingerprint(config['compiler'], root, config.get('sdk'))
    allowed, gate_reason = permitted({**config, 'compiler_fingerprint': info})
    sdk = inspect_sdk(config.get('sdk'))
    unit_configs = {u['id']: u for u in config['units']}
    objects, compile_results, unit_errors = {}, {}, {}
    for uid, unit in unit_configs.items():
        try:
            result = load_json(root / f'build/units/{uid}.compile.json')
            compile_results[uid] = result
            if result['status'] != 'compiled':
                raise ToolError('Candidate compilation failed; see unit diagnostics')
            if result.get('source_policy') != SOURCE_POLICY:
                raise ToolError('Candidate has no current source policy validation; rebuild')
            if result.get('sdk') != sdk:
                raise ToolError('SDK content changed since compilation; rebuild')
            if result['compiler'] != info:
                raise ToolError('Compiler fingerprint changed; reconfigure and rebuild')
            if (result['flags'] != compile_flags(config, unit, normalized=True) or result['source'] != unit['source']
                    or result.get('sdk_required') != bool(unit.get('sdk_required', config['compiler'].get('sdk_required')))):
                raise ToolError('Candidate compile configuration changed; rebuild')
            if result['source_sha256'] != sha256(local_path(root, unit['source'])):
                raise ToolError('Candidate source changed since compilation; rebuild')
            for dependency, digest in result.get('dependencies', {}).items():
                if sha256(local_path(root, dependency)) != digest:
                    raise ToolError('Included dependency changed since compilation; rebuild: ' + dependency)
            objpath = root / result['object']
            if sha256(objpath) != result['object_sha256']:
                raise ToolError('Candidate object fingerprint changed; rebuild')
            unit_allowed, reason = permitted({**config, 'compiler_fingerprint': info}, unit['source'])
            if not unit_allowed:
                raise ToolError(reason)
            objects[uid] = MachO(objpath.read_bytes())
        except (OSError, ToolError, ValueError) as e:
            unit_errors[uid] = str(e)
    from .data import recover_data, compare_data
    data_inventory = recover_data(original, inventory)
    data_results = []
    for record in data_inventory['records']:
        if details_id and details_id != record['id']:
            continue
        uid = record['group_id']
        for mapped_uid, unit in unit_configs.items():
            if record['id'] in unit.get('data', {}):
                uid = mapped_uid
                break
        unit = unit_configs.get(uid, {})
        # A partial source unit must opt in to data explicitly.
        implemented = ('implemented_functions' not in unit or record['id'] in unit.get('data', {}))
        data = compare_data(original, inventory, record, objects.get(uid) if implemented else None,
                            unit.get('data', {}).get(record['id']), unit.get('placements'), bool(details_id) or details)
        if implemented and uid in unit_errors:
            data.update(status='compile_error' if compile_results.get(uid, {}).get('status') != 'compiled' else 'unresolved',
                        reasons=data['reasons'] + [unit_errors[uid]], byte_verified=False)
        data.update(candidate_source=unit.get('source') if implemented else None, candidate_group_id=uid)
        if details:
            data['compiler'] = info
            data['compile'] = compile_results.get(uid)
        if details_id:
            if record['group_id'] == 'unowned-data':
                from .linking import current_state
                linked = current_state(root, config)
                if linked['state'] == 'verified':
                    data.update(status='matched', byte_equal=True, byte_verified=True, candidate_size=data['size'], linked_image_verified=True,
                                reasons=['Verified source-only replacement image establishes this allocation'])
                    for row in data['rows']:
                        for key in ('', '_ascii', '_words'):
                            row['candidate' + key] = row['original' + key]
                        row['different'] = False
            data['compiler'] = info
            data['compile'] = compile_results.get(uid)
            if data['compile']:
                data['diagnostic_text'] = (root / data['compile']['diagnostics']).read_text()
            return data
        data_results.append(data)
    results = []
    for f in inventory['functions']:
        if details_id and f['id'] != details_id:
            continue
        uid = f['group_id']
        u = unit_configs.get(uid, {})
        implemented = 'implemented_functions' not in u or f['id'] in u['implemented_functions']
        compared = compare_function(original, inventory, f, objects.get(uid) if implemented else None, u.get('functions', {}).get(f['id']),
                                    u.get('placements'), bool(details_id) or details)
        if implemented and uid in unit_errors:
            compiled = compile_results.get(uid, {})
            status = 'compile_error' if compiled.get('status') != 'compiled' else 'unresolved'
            compared.update(status=status, reasons=compared['reasons'] + [unit_errors[uid]])
        elif not allowed and compared['status'] != 'missing':
            compared.update(status='unresolved', reasons=compared['reasons'] + [gate_reason])
        compared.update({k: f[k] for k in ('group_id', 'symbol', 'address', 'size', 'mode', 'source_path', 'section', 'ambiguities')})
        compared['section_offset'] = f['address'] - original.section(f['section']).address
        compared['candidate_source'] = u.get('source') if implemented else None
        if implemented and f['id'] in u.get('recovery_kinds', {}):
            compared['recovery_kind'] = u['recovery_kinds'][f['id']]
        compared['byte_verified'] = compared['status'] == 'matched'
        if details:
            compared['compiler'] = info
            compared['compile'] = compile_results.get(uid)
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
    native = native_report(inventory, config, results, units, info)
    from .report import data_metrics
    native['data_inventory_sha256'] = config.get('data_inventory_sha256')
    native['data_coverage'] = data_inventory['coverage']
    native['data_scope'] = data_inventory['scope']
    native['data_sections'] = data_inventory['sections']
    native['data'] = data_results
    native['data_metrics'] = data_metrics(data_results)
    for unit in units:
        from .report import metrics
        unit['metrics'] = metrics([f for f in results if f['group_id'] == unit['id']])
        unit['data_metrics'] = data_metrics([d for d in data_results if d['group_id'] == unit['id']])
    if sdk:
        native['sdk'] = sdk
    if include_link:
        from .linking import current_state
        native['linking'] = current_state(root, config)
        if native['linking']['state'] == 'verified':
            # Whole-image equality establishes shared/linker-generated data
            # which has no object-level STABS ownership or candidate symbol.
            for data in data_results:
                if data['group_id'] == 'unowned-data':
                    data.update(status='matched', byte_equal=True, byte_verified=True, linked_image_verified=True,
                                reasons=['Verified source-only replacement image establishes this allocation'])
                    if details:
                        for row in data['rows']:
                            for key in ('', '_ascii', '_words'):
                                row['candidate' + key] = row['original' + key]
                            row['different'] = False
            native['data_metrics'] = data_metrics(data_results)
        for unit in units:
            verified = native['linking']['state'] == 'verified' and unit['id'] in native['linking'].get('participating_units', [])
            unit['linking'] = {'complete_units': int(verified), 'complete_code': unit['metrics']['matched_bytes'] if verified else 0,
                               'complete_data': unit['data_metrics']['matched_bytes'] if verified else 0}
    return native


def report(root):
    result = comparisons(root)
    write_json(Path(root) / 'build/report.json', result)
    write_json(Path(root) / 'build/objdiff-report.json', objdiff_adapter(result))
    m = result['metrics']
    print(f'{m["matched_functions"]}/{m["total_functions"]} verified functions; {m["matched_bytes"]} matched bytes; '
          f'{m["missing_candidates"]} missing; {m["unresolved_comparisons"]} unresolved; {m["compile_errors"]} compile errors')
    return 1 if m['compile_errors'] or result['data_metrics']['compile_errors'] or any(u['error'] for u in result['units']) or result['linking']['state'] == 'failed' else 0
