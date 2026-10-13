#!/usr/bin/env python3
"""Diagnostic, source-only multi-compiler experiments. Never awards progress credit.

Reuse the original STABS inventory, conservative candidate boundaries and exact
relocation resolver. Missing historical compilers remain explicit unavailable
rows. Commands, preprocessing, assembly, object metadata and comparisons live
under ignored build/; report JSON/Markdown may be published after review.
"""
import argparse
from collections import Counter
from dataclasses import asdict
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.compiler_investigation import evidence, matrix, PROBES
from tools.pirates.compiler import command, language_flags
from tools.pirates.compare import assembly, candidate_span, compare_function
from tools.pirates.inventory import recover
from tools.pirates.macho import MachO
from tools.pirates.sdk import header_flags, require_sdk
from tools.pirates.source_policy import SOURCE_POLICY, validate_source
from tools.pirates.util import ToolError, depfile_paths, load_json, local_path, sha256, write_json

ROOT = Path(__file__).resolve().parents[1]


def capture(argv, root, env=None, timeout=120):
    try:
        p = subprocess.run(argv, cwd=root, env=env, capture_output=True,
                           text=True, errors='replace', timeout=timeout)
        return {'argv': argv, 'returncode': p.returncode,
                'stdout': p.stdout, 'stderr': p.stderr}
    except (OSError, subprocess.TimeoutExpired) as error:
        return {'argv': argv, 'returncode': None, 'error': str(error)}


def compiler_identity(profile, root, sdk):
    cmd = command(profile, root, sdk)
    env = {**os.environ, **profile.get('environment', {}), 'LC_ALL': 'C', 'TZ': 'UTC'}
    result = {'profile': profile, 'version': capture(cmd + ['--version'], root, env),
              'configuration': capture(cmd + ['-v'], root, env),
              'driver': capture(cmd + profile.get('flags', []) + ['-###', '-x', 'c++', '-c', '/dev/null'], root, env)}
    if profile.get('container'):
        result['image'] = profile['container']['image']
        frontend = capture(cmd + ['-print-prog-name=cc1plus'], root, env)
        result['frontend_path'] = frontend
        paths = [cmd[-1]]
        if frontend['returncode'] == 0:
            paths.append(frontend['stdout'].strip())
        result['binary_hashes'] = capture(cmd[:-1] + ['sha256sum'] + paths, root, env)
        result['backend'] = capture(cmd[:-1] + [str(Path(cmd[-1]).parent / 'llc'), '--version'], root, env)
        result['assembler'] = capture(cmd[:-1] + [str(Path(cmd[-1]).parent / 'arm-apple-darwin11-as'), '-v', '-o', '/dev/null'], root, env)
    else:
        binary = shutil.which(cmd[0]) or str(root / cmd[0])
        result['driver_sha256'] = sha256(Path(binary)) if Path(binary).is_file() else None
        result['support_hashes'] = {p: sha256(Path(p)) for p in profile.get('identity_files', []) if Path(p).is_file()}
    if profile['family'] == 'llvmgcc42':
        for name, args in {'triple': ['-dumpmachine'], 'specs': ['-dumpspecs'], 'search_dirs': ['-print-search-dirs']}.items():
            result[name] = capture(cmd + args, root, env)
    result['available'] = result['version']['returncode'] == 0
    return cmd, env, result


def object_metadata(obj):
    return {'filetype': obj.filetype, 'flags': obj.flags, 'uuid': obj.uuid,
            'sections': [asdict(s) for s in obj.sections],
            'symbols': [asdict(s) for s in obj.symbols], 'data_ranges': obj.data_ranges}


def compile_source(profile, cmd, env, source, flags, directory, root, sdk):
    source_path = local_path(root, source)
    # Outputs under workspace also work through the /work container mount.
    directory.mkdir(parents=True, exist_ok=True)
    obj_path, asm_path, dep_path = (directory / n for n in ('candidate.o', 'candidate.s', 'candidate.d'))
    for path in (obj_path, asm_path, dep_path):
        path.unlink(missing_ok=True)
    sdk_flags = header_flags(profile, sdk, source)
    driver = cmd + language_flags(source) + flags + sdk_flags
    record = {'source': source, 'source_sha256': sha256(source_path), 'flags': flags,
              'sdk_flags': sdk_flags, 'source_policy': {'version': SOURCE_POLICY['version'], 'validated': False},
              'status': 'compile_error', 'object': str(obj_path.relative_to(root)), 'dependencies': {}}
    dep_flags = ['-MD', '-MF', str(dep_path.relative_to(root))]
    prep = capture(driver + dep_flags + ['-E', source], root, env)
    (directory / 'preprocess.log').write_text(prep.get('stderr', '') + prep.get('error', ''))
    record['preprocess'] = {k: v for k, v in prep.items() if k != 'stdout'}
    if prep['returncode'] != 0:
        return record, None
    record['preprocessed_sha256'] = hashlib.sha256(prep['stdout'].encode()).hexdigest()
    try:
        validate_source(prep['stdout'])
    except ToolError as error:
        record.update(status='source_policy_rejected', reason=str(error))
        return record, None
    record['source_policy'] = dict(SOURCE_POLICY)
    for name in depfile_paths(dep_path):
        path = Path(name)
        path = path if path.is_absolute() else root / path
        if path.is_file() and path.resolve().is_relative_to(root):
            record['dependencies'][str(path.resolve().relative_to(root))] = sha256(path)
    for name, mode, path in [('assembly', '-S', asm_path), ('compile', '-c', obj_path)]:
        stage = capture(driver + dep_flags + [mode, source, '-o', str(path.relative_to(root))], root, env)
        (directory / (name + '.log')).write_text(stage.get('stdout', '') + stage.get('stderr', '') + stage.get('error', ''))
        record[name] = stage
        if stage['returncode'] != 0:
            obj_path.unlink(missing_ok=True)
            return record, None
    if record['source_sha256'] != sha256(source_path) or any(sha256(root / p) != h for p, h in record['dependencies'].items()):
        obj_path.unlink(missing_ok=True)
        record['reason'] = 'Source or dependency changed during experiment'
        return record, None
    try:
        obj = MachO(obj_path.read_bytes())
        if obj.filetype != 1:
            raise ToolError('Expected relocatable Mach-O')
        record.update(status='compiled', object_sha256=sha256(obj_path),
                      assembly_sha256=sha256(asm_path), metadata=object_metadata(obj))
        return record, obj
    except ToolError as error:
        record['reason'] = str(error)
        return record, None


def pattern(rows):
    instructions = [r['instruction'] for r in rows if r['kind'] == 'instruction']
    return {'instructions': instructions, 'movw_movt': 'movw' in instructions and 'movt' in instructions,
            'ubfx': 'ubfx' in instructions,
            'terminal_b': bool(instructions) and instructions[-1] in ('b', 'b.w'),
            'ordinary_call': any(i in ('bl', 'blx') for i in instructions)}


def summarize(result):
    lines = ['# Compiler comparison', '', result['scope'], '',
             'Original SHA-256: `' + result['original_sha256'] + '`.', '',
             'Source policy runs on each preprocessed TU. All byte comparisons use the existing relocation resolver. Runtime equivalence is untested.', '',
             '| Compiler | Available | Status counts |', '|---|---|---|']
    for name, comp in result['compilers'].items():
        counts = Counter(r['comparison']['status'] for r in result['results'] if r['compiler'] == name)
        lines.append(f"| {name} | {comp.get('available', False)} | {dict(counts)} |")
        if not comp.get('available'):
            lines.append('')
            lines.append(name + ': ' + comp.get('reason', 'Version invocation failed; inspect JSON diagnostics.'))
            lines.append('')
    lines += ['', '| Function / probe | Compiler / flags | Status | Similarity | Reasons |', '|---|---|---|---|---|']
    for row in result['results']:
        comp = row['comparison']
        reason = '; '.join(comp.get('reasons', []))[:240].replace('|', '\\|').replace('\n', ' ')
        lines.append(f"| `{row['id']}` | {row['compiler']} / {row['case']} | {comp['status']} | {comp.get('similarity')} | {reason} |")
    return '\n'.join(lines) + '\n'


def execute(args, root=ROOT):
    root = root.resolve()
    output = args.output.resolve()
    output.relative_to(root)  # Container adapter requires workspace output.
    output.mkdir(parents=True, exist_ok=True)
    registry = load_json(args.registry)['compilers']
    names = list(registry) if args.compilers == 'all' else args.compilers.split(',')
    if any(n not in registry for n in names):
        raise ToolError('Unknown compiler; choose: ' + ', '.join(registry))
    identity = load_json(root / 'config/identity.json')
    original_path = args.input or root / load_json(root / 'build/config.json')['provenance']['input']
    if sha256(original_path) != identity['executable_sha256']:
        raise ToolError('Original executable does not match config/identity.json')
    original = MachO(original_path.read_bytes())
    inventory = recover(original)
    functions = {f['id']: f for f in inventory['functions']}
    groups = {g['id']: g for g in inventory['groups']}
    if args.action == 'probes':
        sample = {'functions': []}
        cases = matrix()
    else:
        sample = load_json(args.sample)
        if args.action == 'compare':
            sample = {**sample, 'functions': [f for f in sample['functions'] if f['id'] == args.function]}
            if not sample['functions']:
                f = functions.get(args.function)
                if not f:
                    raise ToolError('Function absent from original inventory: ' + args.function)
                units = load_json(root / 'config/candidates.json')['units']
                u = next((u for u in units if u['group_id'] == f['group_id'] and
                          ('implemented_functions' not in u or args.function in u['implemented_functions'])), None)
                if not u:
                    raise ToolError('No reconstructed candidate source for function; supply an explicit --sample mapping')
                entry = {'id': args.function, 'group_id': f['group_id'], 'source': u['source'],
                         'flags': u['flags'], 'source_confidence': 'Existing candidate manifest; original source semantics unproven'}
                if args.function in u.get('functions', {}):
                    entry['mapping'] = u['functions'][args.function]
                if 'placements' in u:
                    entry['placements'] = u['placements']
                sample['functions'] = [entry]
        cases = [{'name': 'unit-profile', 'flags': []}]
        if args.flag_search:
            cases += [{'name': 'O1', 'flags': ['-O1']}, {'name': 'O3', 'flags': ['-O3']},
                      {'name': 'Os', 'flags': ['-Os']}, {'name': 'no-postra', 'flags': ['-mllvm', '-post-RA-scheduler=false']},
                      {'name': 'soft', 'flags': ['-mfloat-abi=soft']}, {'name': 'align4', 'flags': ['-falign-functions=4']},
                      {'name': 'ios5.1', 'flags': ['-miphoneos-version-min=5.1']},
                      {'name': 'tail-enabled', 'flags': ['-mllvm', '-arm-tail-calls=true']},
                      {'name': 'portable-O2', 'flags': []}]
    selected = args.cases.split(',') if args.cases else None
    cases = [c for c in cases if not selected or c['name'] in selected]
    if not cases:
        raise ToolError('No selected flag cases')
    sdk = args.sdk.resolve() if args.sdk else None
    sdk_info = require_sdk(sdk) if sdk else None
    result = {'version': 1, 'scope': 'Diagnostic source-only experiment; no decompilation progress credit or universal identity claim.',
              'original_sha256': sha256(original_path), 'inventory_coverage': inventory['coverage'],
              'harness_sha256': sha256(Path(__file__)), 'registry_sha256': sha256(args.registry),
              'matching_engine_sha256': {name: sha256(root / name) for name in ('tools/pirates/compare.py', 'tools/pirates/relocations.py', 'tools/pirates/inventory.py', 'tools/pirates/macho.py', 'tools/pirates/source_policy.py')},
              'sample': sample, 'sdk': sdk_info, 'compilers': {}, 'builds': [], 'results': []}
    for name in names:
        entry = registry[name]
        try:
            profile = load_json(root / entry['profile']) if 'profile' in entry else entry
            cmd, env, compiler = compiler_identity(profile, root, sdk)
            compiler['provenance'] = entry['provenance']
        except (OSError, ValueError, ToolError) as error:
            compiler = {'available': False, 'reason': entry.get('unavailable_reason', str(error)), 'provenance': entry['provenance']}
        result['compilers'][name] = compiler
        if not compiler['available']:
            print(name + ': unavailable', flush=True)
            continue
        cached = {}
        for case in cases:
            entries = sample['functions'] if args.action != 'probes' else [
                {'id': p, 'source': 'toolchain/probes/codegen.cpp', 'flags': [], 'source_confidence': 'Controlled diagnostic probe'} for p in PROBES]
            for entry in entries:
                fid = entry['id']
                if args.action != 'probes' and fid not in functions:
                    raise ToolError('Function absent from original inventory: ' + fid)
                source = entry['source']
                unit_flags = entry.get('flags', [])
                # Explicit portability control, with removed flags recorded.
                if case['name'] == 'portable-O2':
                    unit_flags = [f for f in unit_flags if f != '-fno-toplevel-reorder']
                flags = (case['flags'] if args.action == 'probes' else profile.get('flags', []) + unit_flags + case['flags'])
                key = (source, tuple(flags))
                if key not in cached:
                    tag = hashlib.sha256(json.dumps(key).encode()).hexdigest()[:16]
                    build, obj = compile_source(profile, cmd, env, source, flags, output / name / tag, root, sdk)
                    build.update(compiler=name, case=case['name'], key=tag)
                    result['builds'].append(build)
                    cached[key] = (build, obj)
                    print(name + '/' + case['name'] + ': ' + source + ': ' + build['status'], flush=True)
                build, obj = cached[key]
                row = {'id': fid, 'compiler': name, 'case': case['name'], 'build_key': build['key'],
                       'source_confidence': entry['source_confidence'], 'runtime_equivalence': 'untested',
                       'removed_flags': ['-fno-toplevel-reorder'] if case['name'] == 'portable-O2' and '-fno-toplevel-reorder' in entry.get('flags', []) else [],
                       'original_source_semantics': entry.get('semantics', 'Reconstructed; actual original source unavailable')}
                if args.action == 'probes':
                    comparison = {'status': build['status'], 'reasons': []}
                    if obj:
                        try:
                            span = candidate_span(obj, fid)
                            if span:
                                sym, size = span
                                rows = assembly(obj.bytes_at(sym.value & ~1, size, sym.section), sym.value & ~1, 'thumb' if sym.thumb else 'arm')
                                row.update(assembly=rows, pattern=pattern(rows), size=size)
                            else:
                                comparison['status'] = 'missing'
                        except ToolError as error:
                            comparison.update(status='unresolved', reasons=[str(error)])
                else:
                    f = functions[fid]
                    row.update(symbol=f['symbol'], group_id=f['group_id'], object_group=groups[f['group_id']]['name'], original_size=f['size'])
                    if entry.get('group_id', f['group_id']) != f['group_id']:
                        raise ToolError('Sample mapping crosses original object groups: ' + fid)
                    comparison = compare_function(original, inventory, f, obj, entry.get('mapping'), entry.get('placements'), details=True)
                    if obj is None:
                        comparison.update(status=build['status'], reasons=[build.get('reason', 'See preprocessing/compilation diagnostics')])
                    comparison['exact_relocated_bytes'] = comparison['status'] == 'matched'
                    comparison['progress_credit'] = False
                    row['original_pattern'] = pattern([p['original'] for p in comparison['rows'] if p['original']])
                    row['candidate_pattern'] = pattern([p['candidate'] for p in comparison['rows'] if p['candidate']])
                row['comparison'] = comparison
                result['results'].append(row)
            write_json(output / 'report.json', result)  # Preserve partial evidence if interrupted.
    write_json(output / 'report.json', result)
    (output / 'summary.md').write_text(summarize(result))
    print('Report: ' + str(output / 'report.json'))
    return result


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('action', choices=['compare', 'batch', 'probes', 'evidence'])
    p.add_argument('--registry', type=Path, default=ROOT / 'config/compiler-forensics.json')
    p.add_argument('--compilers', default='all')
    p.add_argument('--sample', type=Path, default=ROOT / 'config/compiler-probes.json')
    p.add_argument('--function')
    p.add_argument('--cases', help='Comma-separated bounded cases (see matrix() or --flag-search)')
    p.add_argument('--flag-search', action='store_true')
    p.add_argument('--input', type=Path)
    p.add_argument('--sdk', type=Path)
    p.add_argument('--output', type=Path, default=ROOT / 'build/compiler-forensics/comparison')
    args = p.parse_args()
    try:
        if args.action == 'compare' and not args.function:
            p.error('compare requires --function')
        if args.action == 'evidence':
            config = load_json(ROOT / 'build/config.json')
            path = args.input or ROOT / config['provenance']['input']
            if sha256(path) != load_json(ROOT / 'config/identity.json')['executable_sha256']:
                raise ToolError('Original hash mismatch')
            args.output.mkdir(parents=True, exist_ok=True)
            _, result = evidence(MachO(path.read_bytes()))
            write_json(args.output / 'original.json', result)
        else:
            execute(args)
    except (ToolError, OSError, ValueError) as error:
        p.exit(1, str(error) + '\n')


if __name__ == '__main__':
    main()
