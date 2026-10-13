#!/usr/bin/env python3
"""Publish compact forensic measurements, excluding original instruction bytes.

Full local listings/objects remain under build/. Publishing never changes the
candidate manifest, compiler validation, original inventory or progress metrics.
"""
import argparse
from collections import Counter
import copy
import hashlib
import json
from pathlib import Path
import sys
import subprocess
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.util import load_json, sha256, ToolError

ROOT = Path(__file__).resolve().parents[1]


def write_json(path, value):
    # Keep each generated record on one line. Markdown is the human report;
    # compact JSON retains every event without hundreds of thousands of
    # indentation-only diff lines. It remains ordinary, parseable JSON.
    def render(item, depth=0):
        pad = '  ' * depth
        if isinstance(item, dict):
            fields = [json.dumps(k) + ': ' + render(v, depth + 1) for k, v in sorted(item.items())]
            return '{\n' + ',\n'.join(pad + '  ' + f for f in fields) + '\n' + pad + '}'
        if isinstance(item, list) and item and isinstance(item[0], dict):
            return '[\n' + ',\n'.join(pad + '  ' + json.dumps(v, sort_keys=True, separators=(',', ':')) for v in item) + '\n' + pad + ']'
        return json.dumps(item, sort_keys=True, separators=(',', ':'))
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(render(value) + '\n')


def scrub(value):
    if isinstance(value, str):
        return value.replace(str(ROOT), '<workspace>')
    if isinstance(value, list):
        return [scrub(v) for v in value]
    if isinstance(value, dict):
        return {k: scrub(v) for k, v in value.items()}
    return value


def compact(report):
    published = {k: report[k] for k in ('version', 'scope', 'original_sha256', 'harness_sha256', 'registry_sha256', 'matching_engine_sha256', 'sdk', 'inventory_coverage') if k in report}
    published['sample_sha256'] = hashlib.sha256(json.dumps(report['sample'], sort_keys=True).encode()).hexdigest()
    published['sample'] = report['sample']
    published['compilers'] = report['compilers']
    published['builds'] = []
    published['dependency_snapshots'] = {}
    for b in report['builds']:
        row = {k: b[k] for k in ('compiler', 'case', 'key', 'source', 'source_sha256', 'preprocessed_sha256',
                                  'flags', 'sdk_flags', 'source_policy', 'status', 'object_sha256', 'assembly_sha256', 'reason') if k in b}
        dependencies = scrub(b.get('dependencies', {}))
        dependency_id = hashlib.sha256(json.dumps(dependencies, sort_keys=True).encode()).hexdigest()
        row['dependency_snapshot'] = dependency_id
        published['dependency_snapshots'][dependency_id] = dependencies
        row['commands'] = {}
        for stage in ('preprocess', 'assembly', 'compile'):
            if stage in b:
                command = b[stage]
                record = {k: command[k] for k in ('argv', 'returncode', 'error') if k in command}
                if command['returncode'] != 0:
                    record['diagnostics'] = command.get('stdout', '') + command.get('stderr', '')
                row['commands'][stage] = record
        published['builds'].append(row)
    published['results'] = []
    for row in report['results']:
        record = {k: v for k, v in row.items() if k not in ('assembly', 'comparison')}
        record['comparison'] = {k: v for k, v in row['comparison'].items() if k not in ('rows', 'relocations')}
        # Relocation target/subtractor addresses remain evidence; instruction
        # words before/after patching do not need publication.
        record['comparison']['relocations'] = [{k: v for k, v in e.items() if k not in ('before', 'after')} for e in row['comparison'].get('relocations', [])]
        for field in ('original_pattern', 'candidate_pattern', 'pattern'):
            if field in record and 'instructions' in record[field]:
                feature = dict(record[field])
                sequence = feature.pop('instructions')
                feature['instruction_counts'] = dict(Counter(sequence))
                feature['instruction_sequence_sha256'] = hashlib.sha256(json.dumps(sequence).encode()).hexdigest()
                if len(sequence) <= 8:
                    feature['instructions'] = sequence
                record[field] = feature
        published['results'].append(record)
    published['status_counts'] = {}
    for row in published['results']:
        key = row['compiler'] + '/' + row['case']
        published['status_counts'].setdefault(key, Counter())[row['comparison']['status']] += 1
    return scrub(published)


def group_family(group):
    path = group['object_path'].lower()
    name = group['name'].lower()
    if 'ogles' in path or name.startswith(('pvrt', 'pvrshell', 'neon')):
        return 'PowerVR'
    if 'tinyxml' in name:
        return 'TinyXML'
    if any(t in name for t in ('phono', 'audio', 'fmod', 'vorbis', 'ogg')):
        return 'Audio'
    if any(t in name for t in ('gamecenter', 'eagl', 'application', 'appdelegate', 'appcontroller')):
        return 'Objective-C/application'
    if 'teminclude' in name or name.startswith(('tempinclude', 'fireinclude', 'ni')):
        return 'Gamebryo/contributed unity'
    if 'iselib' in path or name.startswith('ise'):
        return 'ISE'
    if name.startswith(('pirates', 'ship', 'uiscene', 'uic', 'object3d')):
        return 'Game-specific C++/unity'
    return 'Other/uncertain'


def hypotheses(original, comparison):
    result = []
    for group in original['groups']:
        rows = [r for r in comparison['results'] if r.get('group_id') == group['id']]
        matched = [r for r in rows if r['comparison']['status'] == 'matched']
        evidence = {
            'address_patterns': group.get('pc_relative_movw_movt_functions', 0),
            'terminal_external_b': group.get('terminal_external_b_functions', 0),
            'ubfx': group.get('ubfx_functions', 0),
            'exact_sample_ids_by_compiler': {c: sorted({r['id'] for r in matched if r['compiler'] == c}) for c in {r['compiler'] for r in rows}}
        }
        notes = ['ARM mode established by named symbol metadata; exact compiler/front-end identity and flags remain unidentified.']
        if evidence['address_patterns']:
            notes.append('Contains the conservative PC-relative MOVW/MOVT signature. Current bundled Darwin global lowering is incompatible with the ordinary address probes; nearby Apple LLVM 3.1 source is pattern-compatible.')
        if evidence['terminal_external_b']:
            notes.append('Contains terminal branches to known external definitions. Nearby Clang requires iOS >=5 or explicit arm-tail-calls; current bundled LowerCall disables them. Assembly/source/context alternatives remain possible.')
        if evidence['ubfx']:
            notes.append('UBFX indicates v6t2-compatible selection. It is not a unique compiler fingerprint; current backend can emit it for unoptimized shift pairs.')
        if matched:
            notes.append('Sample matches establish these functions only; partial TUs and source hypotheses prevent whole-group identity attribution.')
        if not rows:
            notes.append('No source benchmark in this group; instruction census only.')
        result.append({'id': group['id'], 'name': group['name'], 'census_reference': group['id'], 'family_hint': group_family(group), 'compiler_identity_confidence': 'unresolved',
                       'mode_confidence': 'high' if group['modes'] else 'no named functions',
                       'settings_hypothesis': {'architecture': 'armv7', 'mode': 'arm' if group['modes'] else None,
                                              'optimization': 'O2 starting point; O3/Os indistinguishable on many small candidates',
                                              'tail_calls': 'group-specific investigation' if evidence['terminal_external_b'] else 'no positive terminal-branch evidence'},
                       'evidence': evidence, 'hypothesis_notes': notes})
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'docs/compiler-forensics/evidence')
    args = parser.parse_args()
    output = args.output
    output.mkdir(parents=True, exist_ok=True)
    for name, path in {
        'benchmark': 'batch/report.json', 'flags': 'flags/report.json',
        'problem-variants': 'problem-variants/report.json', 'clang318-probes': 'clang318-probes-final/report.json'
    }.items():
        report = load_json(ROOT / 'build/compiler-forensics' / path)
        if report['original_sha256'] != load_json(ROOT / 'config/identity.json')['executable_sha256']:
            raise ToolError('Report original identity mismatch: ' + name)
        write_json(output / (name + '.json'), compact(report))
    original = load_json(ROOT / 'build/compiler-forensics/original/original.json')
    # Omit all original disassembly/byte rows from durable census evidence.
    original['examples'] = [{k: v for k, v in e.items() if k != 'assembly'} for e in original['examples']]
    write_json(output / 'original-census.json', original)
    comparison = load_json(output / 'benchmark.json')
    write_json(output / 'object-group-hypotheses.json', hypotheses(original, comparison))
    audit = load_json(ROOT / 'build/compiler-forensics/source-audit.json')
    for revision in audit.values():
        revision.pop('implemented_ids', None)
        revision.pop('screen_rejected_ids', None)
    write_json(output / 'source-audit.json', audit)
    write_json(output / 'execution.json', load_json(ROOT / 'build/compiler-forensics/execution.json'))
    reconstructed = load_json(ROOT / 'build/compiler-forensics/reconstructed-probes-final/report.json')
    reduced = {k: reconstructed[k] for k in ('profile', 'profile_file_sha256', 'source_sha256', 'script_sha256',
                                           'metadata', 'source_policy_validated', 'original_sha256', 'reproducible_O2_object', 'matching_credit')}
    reduced['experiments'] = []
    for e in reconstructed['experiments']:
        r = {k: v for k, v in e.items() if k not in ('probes', 'diagnostic_comparisons')}
        r['patterns'] = {n: {'mode': p.get('mode'), 'size': p.get('size'),
                            'instructions': [i['instruction'] for i in p.get('assembly', []) if i['kind'] == 'instruction']}
                         for n, p in e.get('probes', {}).items()}
        r['diagnostic_comparisons'] = [{k: v for k, v in c.items() if k != 'relocations'} for c in e.get('diagnostic_comparisons', [])]
        reduced['experiments'].append(r)
    write_json(output / 'reconstructed-probes.json', scrub(reduced))
    entries = {r['id']: r for r in comparison['results'] if r['compiler'] == 'reconstructed' and r['case'] == 'unit-profile'}
    names = [e['symbol'][1:] if e['symbol'].startswith('__Z') else e['symbol'] for e in entries.values()]
    try:
        demangled = subprocess.run(['c++filt'], input='\n'.join(names), text=True, capture_output=True, check=True).stdout.splitlines()
    except (OSError, subprocess.CalledProcessError):
        demangled = names
    rows = ['# Representative source benchmark', '',
            '43 functions / 14 original groups / 19,352 original bytes. Existing unit profiles only. Status and resolved assembly similarity are separate from source semantics and runtime equivalence.', '',
            '| Function | Original group | Bytes | Current LLVM-GCC | Nearby Clang 318.0.45 | Modern control |',
            '|---|---|---:|---|---|---|']
    for (fid, entry), label in zip(entries.items(), demangled):
        cells = []
        for compiler in ('reconstructed', 'apple-clang318-source', 'modern-control'):
            r = next(r for r in comparison['results'] if r['id'] == fid and r['compiler'] == compiler and r['case'] == 'unit-profile')
            c = r['comparison']
            cells.append(c['status'] + (f" / {c['similarity']:.2f}%" if c.get('similarity') is not None else ''))
        rows.append('| `' + label.replace('|', '\\|') + '` | ' + entry['object_group'] + ' | ' + str(entry['original_size']) + ' | ' + ' | '.join(cells) + ' |')
    rows += ['', 'All original instructions and full relocated listings remain local. No diagnostic row awards progress credit. Animation source semantics are speculative; see the main report and sample annotations. Genuine Xcode 4.3.2 compilers are unavailable.']
    (output.parent / 'benchmark.md').write_text('\n'.join(rows) + '\n')
    manifest = {p.name: sha256(p) for p in output.glob('*.json') if p.name != 'manifest.json'}
    write_json(output / 'manifest.json', {'version': 1, 'files': manifest,
                                        'note': 'Measurements and hashes only; original instructions, binaries, SDK contents and compiler binaries stay local.'})
    print('Published evidence: ' + str(output))


if __name__ == '__main__':
    main()
