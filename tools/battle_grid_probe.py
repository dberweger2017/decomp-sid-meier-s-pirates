#!/usr/bin/env python3
"""Bounded ARM execution comparison for the connected BattleGrid source cohort.

This is differential evidence under a synthetic ABI environment. It never sets
matching/linking progress, and it is not an iOS runtime or equivalence proof.
"""
import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.build import configuration, comparisons
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, write_json

TARGETS = {
    'square': 'f-c1a99306c51fa2cde8d0',
    'property': 'f-d3e268b9ae2c9bb88404',
    'grid': 'f-bafb440cd2d42578fab8',
    'layer': 'f-86deb9152afba152af34',
}
GRID, HEAP, STACK, STOP, CALLBACK = 0x600000, 0x610000, 0x800000, 0x900000, 0x900100


def environment(layer=2, x=3, y=4, enabled=0, locked=0, empty=False):
    """Observed 32-bit field offsets; no incomplete C++ types are instantiated."""
    state = bytearray(0x20000)
    def put(address, fmt, *values):
        struct.pack_into('<' + fmt, state, address - GRID, *values)
    for i in range(6):
        geometry, data = HEAP + i * 0x100, HEAP + 0x1000 + i * 0x100
        put(GRID + 0x4c48 + i * 4, 'I', geometry if i != 5 else 0)
        put(GRID + 5 + i, 'B', i % 2)
        put(geometry + 0x20, 'H', 0xa0 | (i % 2))
        put(geometry + 0xa4, 'I', HEAP + 0x2000)
        put(geometry + 0xbc, 'I', data)
        put(data + 0x28, 'I', HEAP + 0x3000)
        put(data + 0x32, 'H', 0x20)
        put(data + 0x40, 'I', HEAP + 0x4000)
    # null entry -> wrong type -> material. The only modeled external call is
    # the property virtual Type() operation, with explicitly assigned types.
    put(HEAP + 0x2000, 'III', HEAP + 0x2010, 0, 0)
    put(HEAP + 0x2010, 'III', HEAP + 0x2020, HEAP + 0x2000, HEAP + 0x2100)
    put(HEAP + 0x2020, 'III', 0, HEAP + 0x2010, HEAP + 0x2200)
    for prop in [HEAP + 0x2100, HEAP + 0x2200]:
        put(prop, 'I', HEAP + 0x2300)
        put(prop + 0x58, 'fI', 0.75, 7)
    put(HEAP + 0x2300 + 0x38, 'I', CALLBACK)
    for i in range(16):
        put(HEAP + 0x3000 + i * 16, '4f', i + 0.25, i + 0.5, i + 0.75, 0.875)
        put(HEAP + 0x4000 + i * 2, 'H', (i * 3) % 16)
    put(GRID + 0x3224 + (x * 16 + y) * 2, 'H', enabled)
    put(GRID + 0x3024 + (x * 16 + y) * 2, 'H', locked)
    put(GRID + 0xc + ((layer * 16 + x) * 16 + y) * 4, 'HH', 2, 2 if empty else 9)
    return bytes(state)


def execute(images, operations, initial, property_types=None):
    try:
        import unicorn as uc
        from unicorn import arm_const as arm
    except ImportError as error:
        raise ToolError('Install requirements-emulation.txt to run the ARM probe') from error
    machine = uc.Uc(uc.UC_ARCH_ARM, uc.UC_MODE_ARM)
    machine.ctl_set_cpu_model(arm.UC_CPU_ARM_CORTEX_A8)
    machine.reg_write(arm.UC_ARM_REG_C1_C0_2, 0xf << 20)
    machine.reg_write(arm.UC_ARM_REG_FPEXC, 1 << 30)
    pages = {(entry // 4096) * 4096 for entry, code in images.values()}
    for page in sorted(pages):
        machine.mem_map(page, 4096)
    machine.mem_map(GRID, 0x20000)
    machine.mem_write(GRID, initial)
    machine.mem_map(STACK, 0x10000)
    machine.mem_map(STOP, 4096)
    types = property_types or {HEAP + 0x2100: 1, HEAP + 0x2200: 3}
    callbacks, returns = [], []
    def hook(cpu, address, size, user_data):
        if address == CALLBACK:
            prop = cpu.reg_read(arm.UC_ARM_REG_R0)
            if prop not in types:
                raise ToolError('Unexpected property Type callback receiver')
            callbacks.append(prop)
            cpu.reg_write(arm.UC_ARM_REG_R0, types[prop])
            cpu.reg_write(arm.UC_ARM_REG_PC, cpu.reg_read(arm.UC_ARM_REG_LR))
    machine.hook_add(uc.UC_HOOK_CODE, hook)
    for name, args in operations:
        # Adjacent originals may have larger candidates. Load the current entry
        # only, with its real property callee, so no candidate span overlaps the
        # next entry. Never execute an original fallback for a candidate callee.
        for active in {name, 'property'}:
            entry, code = images[active]
            machine.mem_write(entry, code)
        stack_top = STACK + 0x8000
        for i, register in enumerate([arm.UC_ARM_REG_R0, arm.UC_ARM_REG_R1, arm.UC_ARM_REG_R2, arm.UC_ARM_REG_R3]):
            machine.reg_write(register, args[i] if i < len(args) else 0)
        for i, value in enumerate(args[4:]):
            machine.mem_write(stack_top + i * 4, struct.pack('<I', value))
        machine.reg_write(arm.UC_ARM_REG_SP, stack_top)
        machine.reg_write(arm.UC_ARM_REG_LR, STOP)
        machine.emu_start(images[name][0], STOP, timeout=1000000, count=10000)
        if machine.reg_read(arm.UC_ARM_REG_PC) != STOP:
            raise ToolError('Execution did not return within the instruction/time bound')
        if machine.reg_read(arm.UC_ARM_REG_SP) != stack_top:
            raise ToolError('Callee did not restore SP')
        if name == 'property':
            returns.append(machine.reg_read(arm.UC_ARM_REG_R0))
    return {'memory': bytes(machine.mem_read(GRID, 0x20000)), 'callbacks': callbacks, 'returns': returns}


def scenarios():
    values = []
    for layer in [0, 2, 4]:
        for x, y in [(0, 0), (3, 4), (15, 15)]:
            for before, after, locked, empty in [(0, 1, False, False), (1, 0, False, False),
                                                 (1, 1, False, False), (0, 1, True, False),
                                                 (0, 1, False, True)]:
                mask = 1 << layer
                initial = environment(layer, x, y, mask if before else 0, mask if locked else 0, empty)
                alpha = struct.unpack('<I', struct.pack('<f', 0.375))[0]
                ops = [('layer', [GRID, layer, 0]), ('grid', [GRID, 1]),
                       ('square', [GRID, layer, x, y, after, alpha])]
                values.append((f'layer{layer}-{x}-{y}-{before}{after}-{int(locked)}-{int(empty)}', ops, initial, None))
    for type_id in [1, 3, 9]:
        values.append((f'property-type{type_id}', [('property', [HEAP, type_id])], environment(), None))
    null_list = bytearray(environment())
    struct.pack_into('<I', null_list, HEAP + 0xa4 - GRID, 0)
    values.append(('property-empty-list', [('property', [HEAP, 3])], bytes(null_list), None))
    return values


def compare_execution(original_images, candidate_images, cases):
    results = []
    for name, ops, initial, types in cases:
        before = execute(original_images, ops, initial, types)
        after = execute(candidate_images, ops, initial, types)
        differences = [key for key in before if before[key] != after[key]]
        results.append({'scenario': name, 'passed': not differences, 'differences': differences,
                        'original_memory_sha256': hashlib.sha256(before['memory']).hexdigest(),
                        'candidate_memory_sha256': hashlib.sha256(after['memory']).hexdigest(),
                        'original_callbacks': before['callbacks'], 'candidate_callbacks': after['callbacks'],
                        'original_returns': before['returns'], 'candidate_returns': after['returns']})
    return results


def probe(root):
    config = configuration(root)
    inv = json.loads((root / 'build/inventory.json').read_text())
    original = MachO((root / config['provenance']['input']).read_bytes())
    originals, candidates, records = {}, {}, []
    for name, fid in TARGETS.items():
        f = next(f for f in inv['functions'] if f['id'] == fid)
        detail = comparisons(root, details_id=fid, include_link=False)
        if detail['status'] not in ('matched', 'different') or detail['reasons']:
            raise ToolError(f'{fid}: candidate must compile with fully resolved comparison')
        raw = b''.join(bytes.fromhex(row['candidate']['bytes']) for row in detail['rows'] if row['candidate'])
        if len(raw) != detail['candidate_size']:
            raise ToolError('Incomplete relocated candidate image')
        originals[name] = (f['address'], original.bytes_at(f['address'], f['size']))
        candidates[name] = (f['address'], raw)
        records.append({k: detail[k] for k in ('id', 'symbol', 'size', 'candidate_size', 'status', 'byte_verified', 'similarity', 'relocations')})
    results = compare_execution(originals, candidates, scenarios())
    return {'version': 1, 'scope': 'bounded-differential-execution', 'original_sha256': config['provenance']['executable_sha256'],
            'targets': records, 'scenarios': results, 'passed': all(r['passed'] for r in results),
            'matching_credit': 0, 'replacement_link_credit': 0, 'runtime_validated': False,
            'limitations': 'Synthetic memory/virtual Type callback, valid indices, finite alpha; excludes constructors, ownership, allocators, missing material, complete game and iOS runtime.'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path, default=Path('build/gameplay-stress/execution.json'))
    args = parser.parse_args()
    try:
        result = probe(args.workspace.resolve())
        write_json(args.output, result)
        print(f'{sum(s["passed"] for s in result["scenarios"])}/{len(result["scenarios"])} execution scenarios passed; zero matching/replacement-link credit')
        sys.exit(0 if result['passed'] else 1)
    except (ToolError, OSError, ValueError, KeyError) as error:
        sys.exit(str(error))
