"""Deterministic native function report and objdiff protobuf-JSON adapter."""
from collections import Counter, defaultdict


def covered_bytes(functions):
    ranges = defaultdict(list)
    for f in functions:
        if f['size']:
            ranges[f['section']].append((f['address'], f['address'] + f['size']))
    total = 0
    for spans in ranges.values():
        end = -1
        for a, b in sorted(spans):
            total += max(0, b - max(a, end))
            end = max(end, b)
    return total


def metrics(functions):
    counts = Counter(f['status'] for f in functions)
    matched = [f for f in functions if f['status'] == 'matched']
    similar = [f for f in functions if f['similarity'] is not None]
    denom = covered_bytes(functions)
    return {'total_functions': len(functions), 'total_function_bytes': denom,
            'matched_functions': len(matched), 'matched_bytes': covered_bytes(matched),
            'matched_code_percent': round(100 * covered_bytes(matched) / denom, 4) if denom else None,
            'matched_functions_percent': round(100 * len(matched) / len(functions), 4) if functions else None,
            'missing_candidates': counts['missing'], 'unresolved_comparisons': counts['unresolved'],
            'different_functions': counts['different'], 'compile_errors': counts['compile_error'],
            'similarity_percent': round(sum((f['size'] or 0) * (f['similarity'] or 0) for f in functions) / sum(f['size'] or 0 for f in functions), 4) if any(f['size'] for f in functions) else 0,
            'compared_similarity_percent': round(sum(f['similarity'] for f in similar) / len(similar), 4) if similar else None}


def data_metrics(records):
    counts = Counter(d['status'] for d in records)
    total = covered_bytes(records)
    matched = covered_bytes([d for d in records if d['status'] == 'matched' and d['byte_verified']])
    return {'total_bytes': total, 'matched_bytes': matched, 'matched_percent': round(100 * matched / total, 4) if total else None,
            'total_records': len(records), 'matched_records': counts['matched'], 'missing': counts['missing'],
            'unresolved': counts['unresolved'], 'different': counts['different'], 'compile_errors': counts['compile_error'],
            'zerofill_bytes': sum(d['size'] for d in records if d['zerofill'])}


def native_report(inventory, config, functions, units, compiler):
    return {'version': 2, 'kind': config['provenance']['kind'], 'architecture': 'armv7',
            'input_sha256': config['provenance']['executable_sha256'], 'inventory_sha256': config['inventory_sha256'],
            'coverage': inventory['coverage'], 'compiler': compiler,
            'profile': {k: v for k, v in config['compiler'].items() if k not in ('command', 'validation')},
            'metrics': metrics(functions), 'functions': functions, 'units': units,
            'code_scope': 'union of recovered STABS function ranges, including literal pools; not all executable bytes',
            'similarity_algorithm': 'instruction/operand sequence alignment v1, weighted by STABS record size; overlapping records retain their weights; missing candidates contribute zero',
            'linking': {'supported': False, 'complete_code': 0, 'complete_data': 0, 'complete_units': 0,
                        'reason': 'This milestone compares functions; it does not link a full-game replacement.'}}


def objdiff_adapter(report):
    """Proto JSON field names/types, pinned to the schema in toolchain/sources.lock.json.

    uint64 values are strings as prescribed by protobuf JSON. Completion uses
    the source-only image gates. Adapter fuzzy combines code similarity and
    exact data by bytes; the native code score remains separate.
    """
    groups = defaultdict(list)
    data_groups = defaultdict(list)
    for f in report['functions']:
        groups[f['group_id']].append(f)
    for d in report.get('data', []):
        data_groups[d['group_id']].append(d)

    def measures(functions, units, data=(), complete=None):
        m = metrics(functions)
        total, matched = m['total_function_bytes'], m['matched_bytes']
        n, exact = m['total_functions'], m['matched_functions']
        dm = data_metrics(data)
        complete = complete or {}
        cc, cd = complete.get('complete_code', 0), complete.get('complete_data', 0)
        fuzzy = round((m['similarity_percent'] * total + 100 * dm['matched_bytes']) / (total + dm['total_bytes']), 4) if total + dm['total_bytes'] else 0
        return {'fuzzyMatchPercent': fuzzy, 'totalCode': str(total), 'matchedCode': str(matched),
                'matchedCodePercent': round(100 * matched / total, 4) if total else 0,
                'totalData': str(dm['total_bytes']), 'matchedData': str(dm['matched_bytes']), 'matchedDataPercent': dm['matched_percent'] or 0,
                'totalFunctions': n, 'matchedFunctions': exact,
                'matchedFunctionsPercent': round(100 * exact / n, 4) if n else 0,
                'completeCode': str(cc), 'completeCodePercent': round(100 * cc / total, 4) if total else 0,
                'completeData': str(cd), 'completeDataPercent': round(100 * cd / dm['total_bytes'], 4) if dm['total_bytes'] else 0,
                'totalUnits': units, 'completeUnits': complete.get('complete_units', 0)}

    result = {'version': 2, 'measures': measures(report['functions'], len(report['units']), report.get('data', []), report['linking']), 'categories': [],
            'units': [{'name': u['object_path'], 'measures': measures(groups[u['id']], 1, data_groups[u['id']], u.get('linking')),
                       'sections': [{'name': s['name'], 'size': str(sum(d['size'] for d in data_groups[u['id']] if d['section'] == s['section'])),
                                     'fuzzyMatchPercent': round(100 * sum(d['size'] for d in data_groups[u['id']] if d['section'] == s['section'] and d['byte_verified']) / sum(d['size'] for d in data_groups[u['id']] if d['section'] == s['section']), 4),
                                     'metadata': {'virtualAddress': str(s['address'])}}
                                    for s in report.get('data_sections', []) if any(d['section'] == s['section'] for d in data_groups[u['id']])],
                       'functions': [{'name': f['symbol'], 'size': str(f['size'] or 0),
                                      'fuzzyMatchPercent': f['similarity'] or 0,
                                      'address': str(f['section_offset']), 'metadata': {'virtualAddress': str(f['address'])}}
                                     for f in groups[u['id']]],
                       'metadata': {'complete': bool(u.get('linking', {}).get('complete_units')), 'sourcePath': u.get('source') or u['source_path'],
                                    'autoGenerated': False}} for u in report['units']]}
    if data_groups['unowned-data']:
        shared = data_groups['unowned-data']
        complete = {'complete_data': covered_bytes(shared)} if report['linking'].get('state') == 'verified' else {}
        result['units'].append({'name': '<unattributed data>', 'measures': measures([], 0, shared, complete),
                                'functions': [], 'sections': [{'name': s['name'],
                                  'size': str(sum(d['size'] for d in shared if d['section'] == s['section'])),
                                  'fuzzyMatchPercent': round(100 * sum(d['size'] for d in shared if d['section'] == s['section'] and d['byte_verified']) / sum(d['size'] for d in shared if d['section'] == s['section']), 4)}
                                  for s in report.get('data_sections', []) if any(d['section'] == s['section'] for d in shared)],
                                'metadata': {'complete': False, 'autoGenerated': True, 'sourcePath': ''}})
    return result


def regression(base, head):
    """Fail on loss of verified evidence, build health, or inventory coverage."""
    failures, newly_matched, improvements, regressions = [], [], [], []
    same = ('kind', 'input_sha256', 'inventory_sha256', 'profile', 'compiler')
    for key in same:
        if base[key] != head[key]:
            failures.append('Base/head do not share identical ' + key)
    if base.get('sdk') != head.get('sdk'):
        failures.append('Base/head do not share identical SDK content')
    if base.get('data_coverage') != head.get('data_coverage') or base.get('data_inventory_sha256') != head.get('data_inventory_sha256'):
        failures.append('Data inventory coverage changed')
    old_data = {d['id']: d for d in base.get('data', [])}
    new_data = {d['id']: d for d in head.get('data', [])}
    if set(old_data) - set(new_data):
        failures.append('Data inventory coverage disappeared')
    for did, d in new_data.items():
        if d['status'] == 'compile_error':
            failures.append('Data candidate does not compile: ' + did)
        if old_data.get(did, {}).get('byte_verified') and not d['byte_verified']:
            failures.append('Verified data match regressed: ' + did)
            regressions.append(did)
        if d['byte_verified'] and not old_data.get(did, {}).get('byte_verified'):
            newly_matched.append(did)
    old_link, new_link = base.get('linking', {}), head.get('linking', {})
    if old_link.get('profile') != new_link.get('profile'):
        failures.append('Base/head do not share identical linker profiles')
    if new_link.get('state') == 'failed':
        failures.append('Candidate linking failed; see link diagnostics')
    if old_link.get('state') == 'verified' and new_link.get('state') != 'verified':
        failures.append('Verified replacement image regressed')
    previous = {f['id']: f for f in base['functions']}
    current = {f['id']: f for f in head['functions']}
    if set(previous) - set(current):
        failures.append('Inventory coverage disappeared: ' + ', '.join(sorted(set(previous) - set(current))))
    if base['coverage'] != head['coverage']:
        failures.append('Inventory coverage changed')
    for unit in head['units']:
        if unit.get('error') or (unit.get('compile') or {}).get('status') == 'compile_error':
            failures.append('Candidate unit does not compile: ' + unit['id'])
    if {u['id'] for u in base['units']} - {u['id'] for u in head['units']}:
        failures.append('Original object group coverage disappeared')
    for fid, f in sorted(current.items()):
        old = previous.get(fid)
        if f['status'] == 'compile_error':
            failures.append('Candidate does not compile: ' + fid)
        if old and old['status'] == 'matched' and f['status'] != 'matched':
            failures.append('Verified match regressed: ' + fid)
            regressions.append(fid)
        if f['status'] == 'matched' and (not old or old['status'] != 'matched'):
            newly_matched.append(fid)
        before, after = (old or {}).get('similarity'), f.get('similarity')
        if before is not None and after is not None:
            if after > before:
                improvements.append({'id': fid, 'before': before, 'after': after})
            elif after < before:
                regressions.append(fid)
        elif before is not None and after is None and fid not in regressions:
            regressions.append(fid)
    return {'failures': sorted(set(failures)), 'newly_matched': newly_matched,
            'improvements': improvements, 'regressions': sorted(set(regressions))}


def summary(base, head, delta):
    lines = ['## Pirates! function matching', '', '| Measure | Base | Head |', '|---|---:|---:|']
    for key in ('matched_bytes', 'matched_code_percent', 'matched_functions', 'matched_functions_percent', 'missing_candidates', 'unresolved_comparisons', 'compile_errors', 'similarity_percent'):
        lines.append(f'| {key.replace("_", " ")} | {base["metrics"][key]} | {head["metrics"][key]} |')
    for key in ('total_bytes', 'matched_bytes', 'matched_percent', 'unresolved'):
        lines.append(f'| data {key.replace("_", " ")} | {base.get("data_metrics", {}).get(key, "unavailable")} | {head.get("data_metrics", {}).get(key, "unavailable")} |')
    link = head.get('linking', {})
    lines += ['', 'Code scope: recovered function ranges. Fuzzy similarity is separate from verified byte equality.',
              '', f'Full-game linking: **{link.get("state", "unsupported")}**; {link.get("complete_units", 0)} completed units. {link.get("reason", "")}', '',
              f'New verified matches: {len(delta["newly_matched"])}. Improvements: {len(delta["improvements"])}. Regressions: {len(delta["regressions"])}.', '']
    names = {f['id']: f['symbol'] for f in head['functions']}
    names.update({d['id']: d['symbol'] for d in head.get('data', [])})
    for title, values in [('New verified matches', delta['newly_matched']), ('Regressions', delta['regressions'])]:
        if values:
            lines += [f'### {title}', ''] + ['- `' + v + '` ' + names.get(v, '') for v in values]
    if delta['improvements']:
        lines += ['### Similarity improvements', ''] + [f'- `{v["id"]}`: {v["before"]}% → {v["after"]}%' for v in delta['improvements']]
    if delta['failures']:
        lines += ['', '### Failing checks', ''] + ['- ' + x for x in delta['failures']]
    return '\n'.join(lines) + '\n'
