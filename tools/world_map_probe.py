#!/usr/bin/env python3
"""Execute the complete two-function world-map projection cohort under ARM emulation."""
import argparse
import json
import math
import struct
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.battle_grid_probe import GRID, compare_execution, isolated_arm_images
from tools.pirates.build import configuration, comparisons
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, write_json

TARGETS = {'remap': 'f-5f6877244e1a05413279', 'projection': 'f-7387a1feb235bf2a934a'}


def modf_callback(cpu, arm):
    value = struct.unpack('<d', struct.pack('<II', cpu.reg_read(arm.UC_ARM_REG_R0), cpu.reg_read(arm.UC_ARM_REG_R1)))[0]
    fraction, integral = math.modf(value)
    cpu.mem_write(cpu.reg_read(arm.UC_ARM_REG_R2), struct.pack('<d', integral))
    low, high = struct.unpack('<II', struct.pack('<d', fraction))
    cpu.reg_write(arm.UC_ARM_REG_R0, low)
    cpu.reg_write(arm.UC_ARM_REG_R1, high)


def scenarios():
    cases = []
    bits = lambda f: struct.unpack('<I', struct.pack('<f', f))[0]
    for numerator in [-21, -7, -5, -3, -1, 0, 1, 3, 5, 7, 21]:
        for denominator in [2, 4, 10]:
            args = [numerator & 0xffffffff, 0, denominator, 1, 100]
            cases.append((f'remap-{numerator}-{denominator}', [('remap', args)], bytes(0x20000), None))
    for x, y in [(0., 0.), (462., 293.), (231., 146.5), (13.25, 222.75), (-1.25, 294.25)]:
        for scale in [0.25, 1., 1.5, 4.]:
            args = [bits(x), bits(y), bits(scale), 17, 9, GRID + 0x100, GRID + 0x104]
            cases.append((f'projection-{x}-{y}-{scale}', [('projection', args)], bytes(0x20000), None))
    return cases


def probe(root):
    config = configuration(root)
    inv = json.loads((root / 'build/inventory.json').read_text())
    binary = MachO((root / config['provenance']['input']).read_bytes())
    originals, candidates, records, relocations = {}, {}, [], {}
    for name, fid in TARGETS.items():
        f = next(f for f in inv['functions'] if f['id'] == fid)
        detail = comparisons(root, details_id=fid, include_link=False)
        if detail['status'] not in ('matched', 'different') or detail['reasons']:
            raise ToolError('Projection candidate must compile with resolved relocations: ' + fid)
        raw = b''.join(bytes.fromhex(row['candidate']['bytes']) for row in detail['rows'] if row['candidate'])
        if len(raw) != detail['candidate_size']:
            raise ToolError('Incomplete candidate function image')
        originals[name] = (f['address'], binary.bytes_at(f['address'], f['size']))
        candidates[name] = (f['address'], raw)
        relocations[name] = detail['relocations']
        records.append({k: detail[k] for k in ('id', 'symbol', 'size', 'candidate_size', 'status', 'byte_verified', 'similarity', 'relocations')})
    imports = [s['address'] for s in binary.imported_stubs if s['symbol'] == '_modf']
    if len(imports) != 1:
        raise ToolError('Original modf stub must be unique')
    candidates = isolated_arm_images(candidates, relocations, imports)
    results = compare_execution(originals, candidates, scenarios(), dependencies=('remap',),
                                external_functions={imports[0]: modf_callback}, result_functions=tuple(TARGETS))
    return {'version': 1, 'scope': 'bounded-differential-execution', 'targets': records,
            'original_sha256': config['provenance']['executable_sha256'], 'scenarios': results,
            'passed': all(r['passed'] for r in results), 'matching_credit': 0,
            'replacement_link_credit': 0, 'runtime_validated': False,
            'limitations': 'Finite inputs and defined integer casts/divisions only. modf is a modeled external libc call. Only the two observed projection routines and their output memory are tested; no map rendering, navigation simulation or iOS runtime claim.'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path, default=Path('build/gameplay-stress/projection-execution.json'))
    args = parser.parse_args()
    args.output.unlink(missing_ok=True)
    try:
        result = probe(args.workspace.resolve())
        write_json(args.output, result)
        print(f'{sum(s["passed"] for s in result["scenarios"])}/{len(result["scenarios"])} projection execution scenarios passed; zero matching/replacement-link credit')
        sys.exit(0 if result['passed'] else 1)
    except (ToolError, OSError, ValueError, KeyError) as error:
        write_json(args.output, {'passed': False, 'error': str(error), 'matching_credit': 0, 'replacement_link_credit': 0})
        sys.exit(str(error))
