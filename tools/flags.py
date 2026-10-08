#!/usr/bin/env python3
"""Explicit per-unit flag experiments; never edit the active matching profile."""
import argparse
import re
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.build import configuration, compile_flags
from tools.pirates.compiler import command, fingerprint, permitted
from tools.pirates.sdk import require_sdk
from tools.pirates.macho import MachO
from tools.pirates.compare import compare_function
from tools.pirates.util import ToolError, load_json, write_json, sha256
ROOT = Path(__file__).resolve().parents[1]


def sweep(root, function_id, optimizations=None, modes=None, output=None):
    root = Path(root).resolve()
    config = configuration(root)
    inventory = load_json(root / 'build/inventory.json')
    function = next((f for f in inventory['functions'] if f['id'] == function_id), None)
    if not function:
        raise ToolError('Unknown function ID: ' + function_id)
    unit = next((u for u in config['units'] if u['id'] == function['group_id']), None)
    if not unit:
        raise ToolError('Configure candidate source before investigating its flags')
    info = fingerprint(config['compiler'], root, config.get('sdk'))
    allowed, reason = permitted({**config, 'compiler_fingerprint': info}, unit['source'])
    if not allowed:
        raise ToolError(reason)
    sdk = require_sdk(config['sdk']) if config.get('sdk') else None
    original = MachO((root / config['provenance']['input']).read_bytes())
    cache = root / 'build/flag-experiments' / function_id
    cache.mkdir(parents=True, exist_ok=True)
    base_flags = [flag for flag in compile_flags(config, unit) if not re.fullmatch(r'-O(?:[0-3s]|fast)|-m(?:arm|thumb)', flag)]
    records = []
    for optimization in optimizations or ['-O0', '-O1', '-O2', '-O3', '-Os']:
        if optimization not in ('-O0', '-O1', '-O2', '-O3', '-Os'):
            raise ToolError('Unsupported experiment optimization: ' + optimization)
        for mode in modes or ['arm', 'thumb']:
            if mode not in ('arm', 'thumb'):
                raise ToolError('Unsupported experiment mode: ' + mode)
            flags = base_flags + [optimization, '-m' + mode]
            obj = cache / (optimization[1:] + '-' + mode + '.o')
            obj.unlink(missing_ok=True)
            process = subprocess.run(command(config['compiler'], root, config.get('sdk')) + flags +
                ['-c', unit['source'], '-o', str(obj.relative_to(root))], cwd=root, capture_output=True, text=True, timeout=300)
            (obj.with_suffix('.diagnostics.txt')).write_text(process.stdout + process.stderr)
            record = {'optimization': optimization, 'mode': mode, 'flags': flags, 'returncode': process.returncode}
            if process.returncode:
                record['status'] = 'compile_error'
            else:
                compared = compare_function(original, inventory, function, MachO(obj.read_bytes()),
                                            unit.get('functions', {}).get(function_id), unit.get('placements'), False)
                record.update({k: compared.get(k) for k in ('status', 'similarity', 'candidate_size', 'reasons')})
                record['object_sha256'] = sha256(obj)
            records.append(record)
            print(optimization + ' / ' + mode + ': ' + record['status'], flush=True)
    result = {'function_id': function_id, 'symbol': function['symbol'], 'source': unit['source'],
              'source_sha256': sha256(root / unit['source']), 'compiler': info, 'sdk': sdk,
              'input_sha256': config['provenance']['executable_sha256'], 'experiments': records,
              'conclusion': 'Matches apply to this function only; original per-unit flags and compiler equivalence remain unproven.'}
    write_json(output or cache / 'report.json', result)
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('function_id')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    try:
        sweep(ROOT, args.function_id, output=args.output)
    except (ToolError, OSError, subprocess.SubprocessError) as error:
        sys.exit(str(error))
