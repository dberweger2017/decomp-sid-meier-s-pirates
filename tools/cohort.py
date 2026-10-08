#!/usr/bin/env python3
"""Build a source cohort together, then report fuzzy/exact progress by original group."""
import argparse
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.util import ToolError, load_json, write_json, ninja_command


def select(inventory, name, pattern):
    expression = re.compile(pattern)
    ids = sorted(f['id'] for f in inventory['functions'] if expression.search(f['symbol']))
    if not ids:
        raise ToolError('Selection contains no recovered functions')
    return {'version': 1, 'name': name, 'selection': {'symbol_regex': pattern}, 'function_ids': ids}


def summarize(cohort, inventory, report):
    ids = cohort['function_ids']
    if not ids or len(ids) != len(set(ids)):
        raise ToolError('Cohort must contain distinct function IDs')
    originals = {f['id']: f for f in inventory['functions']}
    results = {f['id']: f for f in report['functions']}
    lost = sorted(set(ids) - (set(originals) & set(results)))
    if lost:
        raise ToolError('Cohort inventory/report coverage disappeared: ' + ', '.join(lost))
    names = {g['id']: g['name'] for g in inventory['groups']}
    groups = {}
    for fid in ids:
        f, r = originals[fid], results[fid]
        if bool(r['byte_verified']) != (r['status'] == 'matched'):
            raise ToolError('Inconsistent verified-match status: ' + fid)
        if r['group_id'] != f['group_id'] or r['size'] != f['size']:
            raise ToolError('Report changed cohort inventory identity: ' + fid)
        group = groups.setdefault(f['group_id'], {'id': f['group_id'], 'name': names[f['group_id']], 'functions': []})
        group['functions'].append({k: r.get(k) for k in ('id', 'symbol', 'size', 'status', 'byte_verified',
                                                      'candidate_source', 'candidate_size', 'similarity', 'reasons')})
    def metrics(functions):
        counts = Counter(f['status'] for f in functions)
        exact = [f for f in functions if f['status'] == 'matched' and f['byte_verified'] and f['candidate_source']]
        resolved = [f for f in functions if f['status'] in ('matched', 'different') and f['similarity'] is not None]
        total = sum(f['size'] for f in functions)
        compared = sum(f['size'] for f in resolved)
        fuzzy_bytes = sum(f['size'] * f['similarity'] / 100 for f in resolved)
        return {'functions': len(functions), 'original_bytes': total, 'status_counts': dict(sorted(counts.items())),
                'exact_functions': len(exact), 'exact_bytes': sum(f['size'] for f in exact),
                'candidate_functions': sum(bool(f['candidate_source']) for f in functions),
                'compared_bytes': compared,
                'compared_byte_weighted_similarity': round(fuzzy_bytes / compared * 100, 4) if compared else None,
                'cohort_byte_weighted_similarity': round(fuzzy_bytes / total * 100, 4) if total else None}
    for group in groups.values():
        group['functions'].sort(key=lambda f: f['id'])
        group['metrics'] = metrics(group['functions'])
        # Largest original differences first, including missing work. Similarity
        # is an assembly alignment heuristic, not an estimated work percentage.
        group['priority_bytes'] = sum(f['size'] * (1 - ((f['similarity'] or 0) if f['status'] in ('matched', 'different') else 0) / 100)
                                      for f in group['functions'] if not f['byte_verified'])
        group['priority_bytes'] = round(group['priority_bytes'], 4)
    ordered = sorted(groups.values(), key=lambda g: (-g['priority_bytes'], g['id']))
    all_functions = [f for g in ordered for f in g['functions']]
    return {'version': 1, 'name': cohort['name'], 'metrics': metrics(all_functions), 'groups': ordered,
            'source_pass_complete': all(bool(f['candidate_source']) and f['status'] in ('matched', 'different') for f in all_functions),
            'replacement_link_credit': 0,
            'limitations': 'Source presence is not source correctness. Fuzzy similarity excludes missing/unresolved cases from the compared denominator, includes them as zero in the cohort denominator, and never earns exact or full-game linking credit.'}


def markdown(result):
    m = result['metrics']
    fmt = lambda value: 'unavailable' if value is None else f'{value:.4f}%'
    lines = [f'## {result["name"]}', '', f'{m["candidate_functions"]}/{m["functions"]} source candidates; '
             f'{m["exact_functions"]} exact functions / {m["exact_bytes"]} bytes.', '',
             f'Compared assembly similarity (weighted by original bytes): {fmt(m["compared_byte_weighted_similarity"])}. '
             f'Whole-cohort similarity (missing/unresolved count as zero): {fmt(m["cohort_byte_weighted_similarity"])}.', '',
             '| Original group | Candidates | Exact bytes | Compared similarity | Remaining difference priority |',
             '|---|---:|---:|---:|---:|']
    for g in result['groups']:
        gm = g['metrics']
        lines.append(f'| {g["name"]} | {gm["candidate_functions"]}/{gm["functions"]} | {gm["exact_bytes"]} | '
                     f'{fmt(gm["compared_byte_weighted_similarity"])} | {g["priority_bytes"]:.4f} |')
    lines += ['', 'Priority is original size × (1 − assembly similarity); it is not a time estimate.', '',
              'Full-game replacement linking receives zero credit from this cohort report.', '']
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    sub = parser.add_subparsers(dest='command', required=True)
    selection = sub.add_parser('select', help='Save an explicit inventory cohort; never generate placeholder source')
    selection.add_argument('--name', required=True)
    selection.add_argument('--symbol-regex', required=True)
    selection.add_argument('--output', type=Path, required=True)
    for name in ['report', 'build']:
        command = sub.add_parser(name, help='Report the configured cohort, optionally rebuilding first')
        command.add_argument('cohort', type=Path)
        command.add_argument('--output', type=Path, required=True)
        command.add_argument('--require-exact', action='store_true',
                             help='Fail after exporting diagnostics unless every selected source entry verifies')
    args = parser.parse_args()
    root = args.workspace.resolve()
    inventory = load_json(root / 'build/inventory.json')
    if args.command == 'select':
        write_json(args.output, select(inventory, args.name, args.symbol_regex))
        return 0
    code = subprocess.run(ninja_command(), cwd=root).returncode if args.command == 'build' else 0
    args.output.unlink(missing_ok=True)
    args.output.with_suffix('.md').unlink(missing_ok=True)
    result = summarize(load_json(args.cohort), inventory, load_json(root / 'build/report.json'))
    write_json(args.output, result)
    args.output.with_suffix('.md').write_text(markdown(result))
    print(markdown(result))
    if args.require_exact and result['metrics']['exact_functions'] != result['metrics']['functions']:
        raise ToolError('Required exact cohort has ' + str(result['metrics']['exact_functions']) +
                        '/' + str(result['metrics']['functions']) + ' verified source entries; report exported')
    return code


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (ToolError, OSError, ValueError, KeyError, re.error) as error:
        sys.exit(str(error))
