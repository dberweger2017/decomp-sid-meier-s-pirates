#!/usr/bin/env python3
"""Read-only source-form audit of candidate manifests at immutable Git revisions.

This conservative local-include screen supplements, never replaces, actual
compiler preprocessing. It reports assembly artifacts separately from source
recovery, without compiling payloads or changing inventory/progress.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import subprocess
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.source_policy import validate_source, SOURCE_POLICY
from tools.pirates.util import ToolError, load_json, write_json

ROOT = Path(__file__).resolve().parents[1]
INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"\n]+)"', re.MULTILINE)


def git(root, *args):
    return subprocess.check_output(['git', *args], cwd=root, text=True)


def audit_revision(root, ref, inventory):
    commit = git(root, 'rev-parse', ref).strip()
    manifest = json.loads(git(root, 'show', commit + ':config/candidates.json'))
    files = set(git(root, 'ls-tree', '-r', '--name-only', commit).splitlines())
    cache = {}
    by_group = {}
    for f in inventory['functions']:
        by_group.setdefault(f['group_id'], []).append(f['id'])

    def visit(path, seen):
        path = str(PurePosixPath(path))
        if path in seen:
            return []
        seen.add(path)
        if path not in files:
            return [{'path': path, 'status': 'missing'}]
        if path not in cache:
            source = git(root, 'show', commit + ':' + path)
            entry = {'path': path, 'sha256': hashlib.sha256(source.encode()).hexdigest(), 'status': 'accepted_source_form'}
            try:
                validate_source(source)
            except ToolError as error:
                entry.update(status='policy_rejected', reason=str(error))
                # Symbolic declaration labels inside unexpanded macros are
                # undecidable before preprocessing, not instruction payloads.
                if re.search(r'__asm__\s*\(\s*[A-Za-z_]\w*\s*\)', source):
                    without_definitions = re.sub(r'^\s*#\s*define(?:[^\n]*\\\n)*[^\n]*', '', source, flags=re.MULTILINE)
                    try:
                        validate_source(without_definitions)
                        entry.update(status='requires_preprocessing', reason='Unexpanded symbolic macro declaration; actual preprocessed TU must decide')
                    except ToolError:
                        pass
            dependencies = []
            for inc in INCLUDE.findall(source):
                # Normalize .. without reading outside the Git tree.
                choices = [str(Path(path).parent / inc), inc, 'src/' + inc]
                normalized = []
                for choice in choices:
                    parts = []
                    for part in PurePosixPath(choice).parts:
                        if part == '..':
                            if parts: parts.pop()
                        elif part != '.': parts.append(part)
                    normalized.append('/'.join(parts))
                found = next((p for p in normalized if p in files), None)
                if found: dependencies.append(found)
            cache[path] = (entry, dependencies)
        entry, dependencies = cache[path]
        return [entry] + [e for p in dependencies for e in visit(p, seen)]

    units = []
    implemented = set()
    for unit in manifest['units']:
        ids = unit.get('implemented_functions', by_group.get(unit['group_id'], []))
        implemented.update(ids)
        closure = visit(unit['source'], set())
        violations = [x for x in closure if x['status'] in ('policy_rejected', 'missing')]
        pending = [x for x in closure if x['status'] == 'requires_preprocessing']
        units.append({'group_id': unit['group_id'], 'source': unit['source'],
                      'functions': ids, 'function_count': len(ids),
                      'status': 'source_form_rejected' if violations else ('requires_preprocessing' if pending else 'source_form_screen_passed'),
                      'requires_preprocessing': pending,
                      'violations': violations, 'local_include_closure': [x['path'] for x in closure]})
    rejected = {fid for u in units if u['status'] == 'source_form_rejected' for fid in u['functions']}
    sizes = {f['id']: f['size'] or 0 for f in inventory['functions']}
    return {'commit': commit, 'source_policy': SOURCE_POLICY, 'units': units,
            'implemented_ids': sorted(implemented), 'screen_rejected_ids': sorted(rejected),
            'summary': {'units': len(units), 'unit_status': dict(Counter(u['status'] for u in units)),
                        'implemented_functions': len(implemented), 'screen_rejected_functions': len(rejected),
                        'screen_rejected_original_bytes': sum(sizes.get(fid, 0) for fid in rejected)},
            'limitations': 'Raw source and literal local includes only. Macro/conditional/SDK includes require compiler preprocessing; passing is not a source-recovery or semantic proof.'}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, default=ROOT / 'build/compiler-forensics/source-audit.json')
    args = parser.parse_args()
    inv = load_json(ROOT / 'build/inventory.json')
    refs = {'pre_pr10': 'c18b410679cdd02187a419a1179b608e1f6581fe',
            'pr10_parent': 'ff8e9be6^', 'pr10': 'ff8e9be6', 'current': 'HEAD'}
    result = {name: audit_revision(ROOT, ref, inv) for name, ref in refs.items()}
    additions = set(result['pr10']['implemented_ids']) - set(result['pr10_parent']['implemented_ids'])
    rejected = set(result['pr10']['screen_rejected_ids'])
    result['pr10_added'] = {'manifest_added_functions': len(additions), 'screen_rejected_additions': len(additions & rejected),
                          'ids': sorted(additions), 'reported_exact_in_pr': 1031,
                          'note': 'Manifest additions differ from PR-reported new exact matches; no machine-code credit inferred by this audit.'}
    write_json(args.output, result)
    print({k: v.get('summary', v) for k, v in result.items() if k != 'pr10_added'})
    print({k: v for k, v in result['pr10_added'].items() if k != 'ids'})


if __name__ == '__main__':
    main()
