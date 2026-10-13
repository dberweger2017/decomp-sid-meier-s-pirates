#!/usr/bin/env python3
"""Bounded ARM execution of the three diagnostic problem-function hypotheses.

Uses original caller bytes and independently compiled, fully relocated candidate
bytes. The matrix callee is a modeled callback, never original code. This tests
caller argument permutation/return, the RTTI address, and the alpha result; it
does not validate matrix arithmetic, RTTI contents, C++ lifetimes, or iOS runtime.
"""
import argparse
import hashlib
from pathlib import Path
import random
import struct
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.battle_grid_probe import GRID, compare_execution
from tools.pirates.macho import MachO
from tools.pirates.util import load_json, sha256, write_json, ToolError

ROOT = Path(__file__).resolve().parents[1]
IDS = {'tail': 'f-8594ffda5a7ad6b10e37', 'rtti': 'f-5a027fbd3240a6a0e288', 'alpha': 'f-4d69d9f014efa7e4d2e9'}


def probe(report_path, compiler, case):
    report = load_json(report_path)
    config = load_json(ROOT / 'build/config.json')
    original_path = ROOT / config['provenance']['input']
    if sha256(original_path) != report['original_sha256']:
        raise ToolError('Original executable fingerprint changed')
    original = MachO(original_path.read_bytes())
    inventory = load_json(ROOT / 'build/inventory.json')
    functions = {f['id']: f for f in inventory['functions']}
    builds = {(b['compiler'], b['key']): b for b in report['builds']}
    result = {'version': 1, 'scope': __doc__, 'compiler': compiler, 'case': case,
              'comparison_report_sha256': sha256(report_path), 'original_sha256': report['original_sha256'],
              'matching_credit': 0, 'replacement_link_credit': 0, 'targets': {}, 'passed': False}
    for name, fid in IDS.items():
        row = next(r for r in report['results'] if r['id'] == fid and r['compiler'] == compiler and r['case'] == case)
        comparison = row['comparison']
        if comparison['status'] not in ('matched', 'different') or comparison['reasons']:
            raise ToolError('Execution requires fully resolved comparison: ' + fid)
        build = builds[(compiler, row['build_key'])]
        if sha256(ROOT / build['source']) != build['source_sha256'] or sha256(ROOT / build['object']) != build['object_sha256']:
            raise ToolError('Candidate source/object fingerprint changed')
        for path, digest in build['dependencies'].items():
            if sha256(ROOT / path) != digest:
                raise ToolError('Candidate dependency changed')
        f = functions[fid]
        if f['mode'] != 'arm':
            raise ToolError('This bounded execution driver requires ARM mode')
        candidate = b''.join(bytes.fromhex(p['candidate']['bytes']) for p in comparison['rows'] if p['candidate'])
        if len(candidate) != comparison['candidate_size']:
            raise ToolError('Incomplete relocated candidate function')
        left = {name: (f['address'], original.bytes_at(f['address'], f['size'], f['section']))}
        right = {name: (f['address'], candidate)}
        initial = bytes(0x20000)
        options = {'dependencies': (), 'result_functions': (name,)}
        if name == 'alpha':
            rng = random.Random(0x112)
            values = sorted({0, 0xffffffff, 0x7fff, 0x8000, 0x8001, 0xffff,
                             *(1 << bit for bit in range(32)),
                             *((1 << bit) - 1 for bit in range(32)),
                             *(rng.getrandbits(32) for _ in range(256))})
            cases = []
            for value in values:
                memory = bytearray(initial)
                struct.pack_into('<I', memory, 44, value)
                cases.append(('flags-' + hex(value), [(name, [GRID])], bytes(memory), None))
            executions = compare_execution(left, right, cases, **options)
            for scenario, value in zip(executions, values):
                scenario['expected_return'] = (value >> 15) & 1
                scenario['passed'] &= scenario['original_returns'] == scenario['candidate_returns'] == [scenario['expected_return']]
        elif name == 'rtti':
            expected = next(s.value for s in original.symbols if s.defined and s.name == '__ZN8NiObject6m_RTTIE')
            executions = compare_execution(left, right, [('named-rtti-address', [(name, [GRID])], initial, None)], **options)
            executions[0]['expected_return'] = expected
            executions[0]['passed'] &= executions[0]['original_returns'] == executions[0]['candidate_returns'] == [expected]
        else:
            branch = next(e for e in comparison['relocations'] if e['type'] == 5 and e['status'] == 'resolved')
            traces = []
            def matrix_callback(cpu, arm):
                args = [cpu.reg_read(r) for r in (arm.UC_ARM_REG_R0, arm.UC_ARM_REG_R1, arm.UC_ARM_REG_R2)]
                traces.append(args)
                # Observable modeled write and legal caller-register clobbers.
                cpu.mem_write(args[2], struct.pack('<II', args[0], args[1]))
                for reg in (arm.UC_ARM_REG_R0, arm.UC_ARM_REG_R1, arm.UC_ARM_REG_R2, arm.UC_ARM_REG_R3, arm.UC_ARM_REG_R12):
                    cpu.reg_write(reg, 0xabc00000)
            options['external_functions'] = {branch['target_address']: matrix_callback}
            executions = compare_execution(left, right,
                [('matrix-caller-permutation', [(name, [GRID, GRID + 0x100, GRID + 0x200])], initial, None)], **options)
            expected = [GRID + 0x100, GRID + 0x200, GRID]
            executions[0]['callback_trace'] = traces
            executions[0]['expected_callback_args'] = expected
            executions[0]['passed'] &= traces == [expected, expected]
        result['targets'][name] = {'id': fid, 'source_confidence': row['source_confidence'],
            'compiled_object_sha256': build['object_sha256'], 'byte_comparison_status': comparison['status'],
            'scenarios': executions, 'passed': all(e['passed'] for e in executions)}
    result['passed'] = all(t['passed'] for t in result['targets'].values())
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--report', type=Path, default=ROOT / 'build/compiler-forensics/problem-variants/report.json')
    parser.add_argument('--compiler', default='apple-clang318-source')
    parser.add_argument('--case', default='ios5.1')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/compiler-forensics/execution.json')
    args = parser.parse_args()
    try:
        result = probe(args.report, args.compiler, args.case)
        write_json(args.output, result)
        print({name: {'passed': t['passed'], 'scenarios': len(t['scenarios'])} for name, t in result['targets'].items()})
        sys.exit(0 if result['passed'] else 1)
    except (ToolError, OSError, ValueError, StopIteration) as error:
        write_json(args.output, {'passed': False, 'error': str(error), 'matching_credit': 0})
        sys.exit(str(error))
