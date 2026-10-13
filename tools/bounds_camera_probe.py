#!/usr/bin/env python3
"""Optional bounded ARM comparison of original geometry and a real subset link.

No instruction rewriting, original-callee fallbacks, or match/link credit.
Only normal-return SDK tan and SjLj registration callbacks are modeled.
"""
import argparse
import hashlib
import json
import math
import random
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.build import configuration
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, write_json

HEAP, STACK, STOP = 0x06000000, 0x08000000, 0x09000000
BOX, MATRIX, VECTOR, OUTPUT = HEAP + 0x200, HEAP + 0x300, HEAP + 0x380, HEAP + 0x400
PLANE, CENTER, HALF = HEAP + 0x500, HEAP + 0x540, HEAP + 0x580
SYMBOLS = {
    'matrix': '__Z22NEON_Matrix4Vector4MulPKfS0_Pf',
    'transform': '__ZN3ISE10ISEAABBBox9transformERK8PVRTMat4',
    'visible': '__ZN3ISE9ISECamera9IsVisibleERNS_10ISEAABBBoxE',
    'frustum': '__ZN3ISE9ISECamera18updateFrustumPlaneEv',
    'classify': '__ZN3ISE8ISEPlane13ClassifyPointERKNS_10ISEVector3ES3_',
    'triangle': '__ZN3ISE8ISEPlane18CreateFromTriangleERNS_10ISEVector3ES2_S2_',
    'normalize': '__ZN3ISE10ISEVector39NormalizeEv',
}
MODELED = {'_tan', '__Unwind_SjLj_Register', '__Unwind_SjLj_Unregister'}


def execute(image, entries, ranges, imports, initial, operations):
    try:
        import unicorn as uc
        from unicorn import arm_const as arm
    except ImportError as error:
        raise ToolError('Install requirements-emulation.txt to run the ARM probe') from error
    cpu = uc.Uc(uc.UC_ARCH_ARM, uc.UC_MODE_ARM)
    cpu.ctl_set_cpu_model(arm.UC_CPU_ARM_CORTEX_A8)
    cpu.reg_write(arm.UC_ARM_REG_C1_C0_2, 0xf << 20)
    cpu.reg_write(arm.UC_ARM_REG_FPEXC, 1 << 30)
    sections = [s for s in image.sections if s.segment in ('__TEXT', '__DATA') and s.size]
    pages = {p for s in sections for p in range(s.address & ~4095, (s.address + s.size + 4095) & ~4095, 4096)}
    # Coalesce adjacent pages: mapping a large original image one page at a
    # time is needlessly expensive in Unicorn's region index.
    ordered = sorted(pages)
    start = end = ordered[0]
    for page in ordered[1:] + [ordered[-1] + 8192]:
        if page != end + 4096:
            cpu.mem_map(start, end + 4096 - start)
            start = page
        end = page
    for section in sections:
        if not section.zerofill:
            cpu.mem_write(section.address, image.data[section.offset:section.offset + section.size])
    cpu.mem_map(HEAP, 4096)
    cpu.mem_write(HEAP, bytes(initial))
    cpu.mem_map(STACK, 0x10000)
    cpu.mem_map(STOP, 4096)
    active_frames, calls, returns = [], [], []

    def hook(machine, address, size, unused):
        if address in imports:
            name = imports[address]
            if name not in MODELED:
                raise ToolError('Unmodeled SDK/exception call: ' + name)
            if name == '_tan':
                bits = struct.pack('<II', machine.reg_read(arm.UC_ARM_REG_R0), machine.reg_read(arm.UC_ARM_REG_R1))
                value = math.tan(struct.unpack('<d', bits)[0])
                lo, hi = struct.unpack('<II', struct.pack('<d', value))
                machine.reg_write(arm.UC_ARM_REG_R0, lo)
                machine.reg_write(arm.UC_ARM_REG_R1, hi)
            elif name.endswith('_Register'):
                active_frames.append(machine.reg_read(arm.UC_ARM_REG_R0))
            else:
                frame = machine.reg_read(arm.UC_ARM_REG_R0)
                if not active_frames or active_frames.pop() != frame:
                    raise ToolError('Unbalanced SjLj frame lifetime')
            # Model legal SDK clobbers so values cannot survive a call merely
            # because the synthetic callback happened to leave them intact.
            for register in (arm.UC_ARM_REG_R2, arm.UC_ARM_REG_R3, arm.UC_ARM_REG_R12):
                machine.reg_write(register, 0x5a5a5a5a)
            if name != '_tan':
                machine.reg_write(arm.UC_ARM_REG_R0, 0x5a5a5a5a)
                machine.reg_write(arm.UC_ARM_REG_R1, 0x5a5a5a5a)
            for index in range(16):
                machine.reg_write(getattr(arm, 'UC_ARM_REG_S' + str(index)), 0x5a5a5a5a)
            calls.append(name)
            machine.reg_write(arm.UC_ARM_REG_PC, machine.reg_read(arm.UC_ARM_REG_LR))
        elif not any(start <= address < end for start, end in ranges):
            raise ToolError(f'Execution left the recovered source closure at {address:#x}')

    cpu.hook_add(uc.UC_HOOK_CODE, hook)
    registers = [arm.UC_ARM_REG_R0, arm.UC_ARM_REG_R1, arm.UC_ARM_REG_R2, arm.UC_ARM_REG_R3]
    for name, args, expected in operations:
        top = STACK + 0x8000
        for index, register in enumerate(registers):
            cpu.reg_write(register, args[index] if index < len(args) else 0)
        for index, value in enumerate(args[4:]):
            cpu.mem_write(top + 4 * index, struct.pack('<I', value))
        cpu.reg_write(arm.UC_ARM_REG_SP, top)
        cpu.reg_write(arm.UC_ARM_REG_LR, STOP)
        try:
            cpu.emu_start(entries[SYMBOLS[name]], STOP, timeout=1000000, count=50000)
        except uc.UcError as error:
            raise ToolError(f'ARM execution failed in {name}: {error}') from error
        if cpu.reg_read(arm.UC_ARM_REG_PC) != STOP or cpu.reg_read(arm.UC_ARM_REG_SP) != top:
            raise ToolError('Execution bound or stack restoration failed: ' + name)
        if active_frames:
            raise ToolError('Leaked normal-return SjLj frame: ' + name)
        if expected is not None or name in ('visible', 'classify'):
            actual = cpu.reg_read(arm.UC_ARM_REG_R0)
            if expected is not None and actual != expected:
                raise ToolError(f'{name}: returned {actual}, expected {expected}')
            returns.append(actual)
    return bytes(cpu.mem_read(HEAP, 4096)), returns, calls


def environment():
    data = bytearray([0xa5] * 4096)
    return data


def put(data, address, values):
    struct.pack_into('<' + str(len(values)) + 'f', data, address - HEAP, *values)


def box(data, lower, upper, empty=False):
    put(data, BOX, (*upper, *lower, *upper, *lower))
    data[BOX - HEAP + 48] = int(empty)


def cases():
    result = []
    identity = [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
    matrices = {
        'identity': identity,
        'translation': [*identity[:12], 3, -4, 2, 1],
        'reflection-scale': [-2, 0, 0, 0, 0, 3, 0, 0, 0, 0, -0.5, 0, 0, 0, 0, 1],
        'rotate-shear': [0, 1, 0, 0, -1, 0.5, 0, 0, 0.25, 0, 1, 0, 3, -2, 4, 1],
        'no-w-division': [1, 0, 0, 0.25, 0, 1, 0, 0, 0, 0, 1, 0, 3, -4, 2, 2],
    }
    for name, matrix in matrices.items():
        for alias in ['none', 'vector', 'matrix']:
            data = environment()
            vector = [2, -3, 4, 1]
            put(data, MATRIX, matrix)
            put(data, VECTOR, vector)
            output = {'none': OUTPUT, 'vector': VECTOR, 'matrix': MATRIX}[alias]
            expected = [sum(matrix[4 * j + i] * vector[j] for j in range(4)) for i in range(4)]
            result.append((f'matrix-{name}-{alias}', data, [('matrix', [MATRIX, VECTOR, output], None)],
                           [(output, 4)], {output: expected}, []))
        for empty in [False, True]:
            data = environment()
            lower, upper = (-2, -1, -3), (4, 5, 2)
            box(data, lower, upper, empty)
            put(data, MATRIX, matrix)
            points = [[sum(matrix[4 * j + i] * v for j, v in enumerate((*corner, 1))) for i in range(3)]
                      for x in [lower[0], upper[0]] for y in [lower[1], upper[1]] for z in [lower[2], upper[2]]
                      for corner in [(x, y, z)]]
            expected = [*upper, *lower] if empty else [*(max(p[i] for p in points) for i in range(3)),
                                                                      *(min(p[i] for p in points) for i in range(3))]
            # Applying the same transform twice must use saved bounds again.
            result.append((f'transform-{name}-empty{int(empty)}', data,
                           [('transform', [BOX, MATRIX], None)] * 2,
                           [(BOX, 6)], {BOX: expected}, [] if empty else [BOX + 48]))
    generator = random.Random(20261013)
    for index in range(24):
        data = environment()
        matrix = [generator.uniform(-3, 3) for _ in range(16)]
        vector = [generator.uniform(-5, 5) for _ in range(4)]
        # The independent reference uses the stored float32 inputs.
        put(data, MATRIX, matrix)
        put(data, VECTOR, vector)
        matrix = struct.unpack_from('<16f', data, MATRIX - HEAP)
        vector = struct.unpack_from('<4f', data, VECTOR - HEAP)
        expected = [sum(matrix[4 * j + i] * vector[j] for j in range(4)) for i in range(4)]
        result.append((f'matrix-seeded-{index}', data, [('matrix', [MATRIX, VECTOR, OUTPUT], None)],
                       [(OUTPUT, 4)], {OUTPUT: expected}, []))
    for axis in range(3):
        for sign in [-1, 1]:
            for distance, expected in [(-2, 1), (-1, 2), (0, 2), (1, 2), (2, 0)]:
                data = environment()
                normal = [0, 0, 0]
                normal[axis] = sign
                center = [0, 0, 0]
                center[axis] = distance * sign
                put(data, PLANE, [*normal, 0])
                put(data, CENTER, center)
                put(data, HALF, [-1, -1, -1])  # Radius uses absolute products.
                result.append((f'plane-{axis}-{sign}-{distance}', data,
                               [('classify', [PLANE, CENTER, HALF], expected)], [], {}, []))
    data = environment()
    put(data, PLANE, [1, 0, 0, 0])
    put(data, CENTER, [float('nan'), 0, 0])
    put(data, HALF, [1, 1, 1])
    result.append(('plane-nan-comparisons', data, [('classify', [PLANE, CENTER, HALF], 2)], [], {}, []))
    for vector in [(0, 0, 0), (3, 4, 0), (-2, 3, -6)]:
        data = environment()
        put(data, VECTOR, vector)
        length = math.sqrt(sum(v * v for v in vector)) or 1
        result.append((f'normalize-{vector}', data, [('normalize', [VECTOR], None)], [(VECTOR, 3)],
                       {VECTOR: [v / length for v in vector]}, []))
    for reverse in [False, True]:
        data = environment()
        put(data, CENTER, [0, 0, 2])
        put(data, HALF, [1, 0, 2])
        put(data, VECTOR, [0, 1, 2])
        b, c = (VECTOR, HALF) if reverse else (HALF, VECTOR)
        expected = [0, 0, 1, -2] if reverse else [0, 0, -1, 2]
        result.append((f'triangle-winding-{reverse}', data, [('triangle', [PLANE, CENTER, b, c], None)],
                       [(PLANE, 4)], {PLANE: expected}, []))
    cube = [(1, 0, 0, 1), (-1, 0, 0, 1), (0, 1, 0, 1), (0, -1, 0, 1), (0, 0, 1, 1), (0, 0, -1, 1)]
    for axis in range(3):
        for value, expected in [(-3, 0), (-1.5, 1), (0, 1), (1.5, 1), (3, 0)]:
            data = environment()
            for index, plane in enumerate(cube):
                put(data, HEAP + 0x160 + 16 * index, plane)
            data[0x1c0] = 0
            center = [0, 0, 0]
            center[axis] = value
            box(data, [c - 0.5 for c in center], [c + 0.5 for c in center])
            result.append((f'cached-frustum-{axis}-{value}', data, [('visible', [HEAP, BOX], expected)], [], {}, []))
    for shifted in [False, True]:
        position = [5, -3, 2] if shifted else [0, 0, 0]
        for center, expected in [((0, 0, -5), 1), ((0, 0, -11), 0), ((0, 0, 0), 0),
                                 ((8, 0, -5), 0), ((-8, 0, -5), 0), ((0, 7, -5), 0),
                                 ((0, -7, -5), 0), ((0, 0, -1), 1), ((0, 0, -10), 1)]:
            data = environment()
            put(data, HEAP + 0x124, position)
            put(data, HEAP + 0x130, [position[0], position[1], position[2] - 1])
            put(data, HEAP + 0x13c, [0, 1, 0])
            put(data, HEAP + 0x148, [math.pi / 2])
            put(data, HEAP + 0x150, [1, 10])
            data[0x1c0] = 1
            world = [c + p for c, p in zip(center, position)]
            box(data, [c - 0.125 for c in world], [c + 0.125 for c in world])
            # Second call takes the clean cache path; all unrelated camera/box
            # storage must survive both calls byte for byte.
            result.append((f'dirty-frustum-{shifted}-{center}', data,
                           [('visible', [HEAP, BOX], expected)] * 2,
                           [(HEAP + 0x160, 24)], {}, [HEAP + 0x1c0]))
    for direction, up, label in [((1, 2, -3), (0, 1, 0), 'tilted'),
                                 ((0, 0, 0), (0, 1, 0), 'zero-forward'),
                                 ((0, 0, -1), (0, 0, -1), 'parallel-up')]:
        position = (5, -3, 2)
        length = math.sqrt(sum(v * v for v in direction)) or 1
        for distance in [0, 10, 25]:
            data = environment()
            put(data, HEAP + 0x124, position)
            put(data, HEAP + 0x130, [p + d for p, d in zip(position, direction)])
            put(data, HEAP + 0x13c, up)
            put(data, HEAP + 0x148, [1.1])
            put(data, HEAP + 0x150, [2, 20])
            data[0x1c0] = 1
            center = [p + d * distance / length for p, d in zip(position, direction)]
            box(data, [c - 0.125 for c in center], [c + 0.125 for c in center])
            expected = int(distance == 10) if label == 'tilted' else 1
            result.append((f'frustum-{label}-{distance}', data, [('visible', [HEAP, BOX], expected)] * 2,
                           [(HEAP + 0x160, 24)], {}, [HEAP + 0x1c0]))
    return result


def probe(root):
    config = configuration(root)
    original = MachO((root / config['provenance']['input']).read_bytes())
    path = root / 'build/recovery-link/workspace/build/link/Pirates'
    candidate = MachO(path.read_bytes())
    status = json.loads((root / 'build/recovery-link/status.json').read_text())
    if status['state'] != 'linked' or status['image_sha256'] != hashlib.sha256(candidate.data).hexdigest():
        raise ToolError('A fresh, successful diagnostic subset link is required')
    for name, digest in status['inputs'].items():
        if name.startswith('src/') and hashlib.sha256((root / name).read_bytes()).hexdigest() != digest:
            raise ToolError('Diagnostic link has stale source inputs: ' + name)
    manifest = json.loads((root / 'config/candidates.json').read_text())
    unwind_tables = []
    for gid in ['o-9cdb54dcd3500115b6d3', 'o-94d8c4649937bf6c3116']:
        unit = next(u for u in manifest['units'] if u['group_id'] == gid)
        obj = MachO((root / 'build/units' / (gid + '.o')).read_bytes())
        section = next(s for s in obj.sections if s.key == '__TEXT,__gcc_except_tab')
        raw = obj.bytes_at(section.address, section.size, section.index)
        address = unit['placements']['sections'][section.key]
        if raw != original.bytes_at(address, len(raw)):
            raise ToolError('Compiler-generated exception table no longer matches original: ' + gid)
        unwind_tables.append({'group_id': gid, 'address': address, 'size': len(raw),
                              'sha256': hashlib.sha256(raw).hexdigest()})
    inventory = json.loads((root / 'build/inventory.json').read_text())
    targets = {s.name: s.value for s in candidate.symbols if s.defined and s.section == 1
               and s.name != '__mh_execute_header'}
    originals = {f['symbol']: f for f in inventory['functions']}
    if not set(SYMBOLS.values()) <= set(targets):
        raise ToolError('Subset link does not contain this probe cohort')
    original_entries = {name: originals[name]['address'] for name in targets}
    original_ranges = [(f['address'], f['address'] + f['size']) for name, f in originals.items() if name in targets]
    candidate_ranges = [(s.address, s.address + s.size) for s in candidate.sections if s.name == '__text']
    original_imports = {s['address']: s['symbol'] for s in original.imported_stubs}
    # ld64 emits four-byte ARM ldr-pc stubs here. Verify their opcode and slot's
    # actual dynamic binding instead of trusting the order of undefined names.
    bindings = {b['address']: b['symbol'] for b in status['structure']['bindings'] if b['kind'] == 'lazy'}
    candidate_imports = {}
    for section in candidate.sections:
        if section.flags & 0xff == 8:
            for address in range(section.address, section.address + section.size, 4):
                if candidate.bytes_at(address, 4) != struct.pack('<I', 0xe59ff00c) or address + 20 not in bindings:
                    raise ToolError('Unsupported diagnostic import stub')
                candidate_imports[address] = bindings[address + 20]
    results, maximum_error = [], 0.0
    for case_index, (name, initial, operations, float_ranges, expected, writable_bytes) in enumerate(cases()):
        before, returns, calls = execute(original, original_entries, original_ranges, original_imports, initial, operations)
        after, new_returns, new_calls = execute(candidate, targets, candidate_ranges, candidate_imports, initial, operations)
        allowed = {address - HEAP for address in writable_bytes}
        differences = []
        for address, count in float_ranges:
            start = address - HEAP
            allowed.update(range(start, start + count * 4))
            left = struct.unpack_from('<' + str(count) + 'f', before, start)
            right = struct.unpack_from('<' + str(count) + 'f', after, start)
            for index, (a, b) in enumerate(zip(left, right)):
                maximum_error = max(maximum_error, abs(a - b))
                if not math.isclose(a, b, rel_tol=2e-6, abs_tol=2e-6):
                    differences.append(f'float {address + index * 4:#x}: {a} != {b}')
                if address in expected and not math.isclose(a, expected[address][index], rel_tol=2e-6, abs_tol=2e-6):
                    differences.append(f'original disagrees with fixture reference at {address + index * 4:#x}')
        if returns != new_returns:
            differences.append('return values')
        for address in writable_bytes:
            offset = address - HEAP
            if before[offset] != after[offset]:
                differences.append(f'output byte {address:#x}')
        if any(before[i] != initial[i] or after[i] != initial[i] for i in range(4096) if i not in allowed):
            differences.append('unexpected writes outside declared outputs')
        results.append({'scenario': name, 'passed': not differences, 'differences': differences,
                        'original_returns': returns, 'candidate_returns': new_returns,
                        'original_sdk_calls': calls, 'candidate_sdk_calls': new_calls})
        if (case_index + 1) % 25 == 0:
            print(f'Compared {case_index + 1} ARM fixtures', flush=True)
    return {'version': 1, 'passed': all(r['passed'] for r in results), 'scenarios': results,
            'maximum_absolute_float_difference': maximum_error,
            'original_sha256': hashlib.sha256(original.data).hexdigest(),
            'candidate_link_sha256': hashlib.sha256(candidate.data).hexdigest(),
            'verified_exception_tables': unwind_tables,
            'matching_credit': 0, 'replacement_link_credit': 0, 'runtime_validated': False,
            'limitations': 'Finite fixtures plus one NaN classification; synthetic memory and normal-return SDK models. '
                           'No exception unwinding, iOS runtime, complete camera class, or whole-game equivalence.'}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path, default=Path('build/bounds-camera-execution.json'))
    args = parser.parse_args()
    args.output.unlink(missing_ok=True)
    try:
        result = probe(args.workspace.resolve())
        write_json(args.output, result)
        print(f'{sum(r["passed"] for r in result["scenarios"])}/{len(result["scenarios"])} ARM scenarios passed; '
              f'maximum float difference {result["maximum_absolute_float_difference"]}; zero matching/link credit')
        sys.exit(0 if result['passed'] else 1)
    except (ToolError, OSError, ValueError, KeyError) as error:
        write_json(args.output, {'passed': False, 'error': str(error), 'matching_credit': 0, 'replacement_link_credit': 0})
        sys.exit(str(error))
