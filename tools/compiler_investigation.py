#!/usr/bin/env python3
"""Diagnostic code-generation matrix and original-binary evidence. No match credit.

Runs through an explicit compiler profile, retaining commands, errors, object
hashes and disassembly. It does not change compiler validation, active profiles,
candidate manifests, source policy or matching progress.
"""
import argparse
from collections import Counter, defaultdict
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.compiler import command
from tools.pirates.compare import assembly, candidate_span, compare_function
from tools.pirates.inventory import recover
from tools.pirates.macho import MachO
from tools.pirates.source_policy import validate_source
from tools.pirates.util import ToolError, load_json, sha256, write_json

ROOT = Path(__file__).resolve().parents[1]
EXAMPLES = (
    '__Z19PVRTMatrixMultiplyFR11PVRTMATRIXfRKS_S2_',
    '__ZNK8NiObject7GetRTTIEv',
    '__ZN3ISE10ISETexture14IsAlphaEnabledEv',
)
PROBES = ('_tail_int', '_tail_void', '_global_address', '_hidden_address',
          '_defined_address',
          '_extract_bit', '_extract_field', '_extract_shift_pair',
          '_extract_bitfield', '_constant_value', '_add_one') + EXAMPLES


def matrix():
    """Explicit bounded hypotheses; unsupported flags remain visible failures."""
    base = ['-march=armv7-a', '-mfloat-abi=softfp',
            '-miphoneos-version-min=4.2', '-marm']
    cases = [{'name': 'arm-' + opt[1:], 'flags': base + [opt]}
             for opt in ('-O0', '-O1', '-O2', '-O3', '-Os')]
    variants = {
        'thumb': ['-mthumb'],
        'cortex-a8': ['-mcpu=cortex-a8'],
        'cortex-a9': ['-mcpu=cortex-a9'],
        'soft': ['-mfloat-abi=soft'],
        'neon': ['-mfpu=neon'],
        'pic': ['-fPIC'],
        'no-pic': ['-fno-pic'],
        'dynamic-no-pic': ['-mdynamic-no-pic'],
        'hidden': ['-fvisibility=hidden'],
        'no-postra': ['-mllvm', '-post-RA-scheduler=false'],
        'no-schedule': ['-fno-schedule-insns', '-fno-schedule-insns2'],
        'sibling-calls': ['-foptimize-sibling-calls'],
        'llvm-tailcall': ['-mllvm', '-tailcallopt'],
        'movt-on': ['-mllvm', '-arm-use-movt=true'],
        'movt-off': ['-mllvm', '-arm-use-movt=false'],
        'thumb-movt-off': ['-mthumb', '-mllvm', '-arm-use-movt=false'],
        'no-rtti-exceptions': ['-fno-rtti', '-fno-exceptions'],
        'deployment-5.1': ['-miphoneos-version-min=5.1'],
        'arm-tail-calls': ['-mllvm', '-arm-tail-calls=true'],
        'darwin-movt-off': ['-mllvm', '-arm-darwin-use-movt=false'],
        'darwin-movt-on': ['-mllvm', '-arm-darwin-use-movt=true'],
        'aapcs': ['-mabi=aapcs'],
        'apcs-gnu': ['-mabi=apcs-gnu'],
        'arch-armv7': ['-march=armv7'],
        'xcode-arch-armv7': ['-arch', 'armv7'],
    }
    cases += [{'name': name, 'flags': base + ['-O2'] + flags}
              for name, flags in variants.items()]
    # Defaults must be tested without silently inheriting the research profile.
    cases += [{'name': 'driver-default', 'flags': ['-O2']},
              {'name': 'cpu-only-arm', 'flags': ['-O2', '-mcpu=cortex-a8', '-marm']},
              {'name': 'xcode-arch-only', 'flags': ['-O2', '-arch', 'armv7']}]
    return cases


def evidence(original):
    inventory = recover(original)
    groups = {g['id']: g for g in inventory['groups']}
    targets = {f['address'] for f in inventory['functions']}
    targets.update(s.value & ~1 for s in original.symbols if s.defined)
    targets.update(s['address'] for s in original.imported_stubs)
    result = {'executable_sha256': hashlib.sha256(original.data).hexdigest(),
              'coverage': inventory['coverage'],
              'modes': dict(Counter(f['mode'] for f in inventory['functions'])),
              'scope': 'Named STABS function ranges only; literal-pool detection is a display heuristic.',
              'terminal_branch_definition': 'Last decoded instruction is unconditional B to a known definition outside the function. Tail-transfer candidate, not compiler attribution.',
              'groups': [], 'examples': []}
    features = defaultdict(Counter)
    oso = {s.name: s.value for s in original.symbols if s.type == 0x66}
    result['stabs_kinds'] = dict(Counter(hex(s.type) for s in original.symbols if s.type & 0xe0))
    result['compiler_producer_records'] = [{'name': s.name, 'value': s.value} for s in original.symbols if s.type == 0x3c or any(t in s.name.lower() for t in ('gcc2_compiled', 'llvm', 'clang'))]
    for f in inventory['functions']:
        rows = assembly(original.bytes_at(f['address'], f['size'], f['section']),
                        f['address'], f['mode'])
        instructions = [r for r in rows if r['kind'] == 'instruction']
        counts = Counter(r['instruction'] for r in instructions)
        c = features[f['group_id']]
        c['functions'] += 1
        for mnemonic in ('movw', 'movt', 'ubfx', 'sbfx', 'bfi', 'bl', 'blx', 'vadd.f32', 'vmul.f32', 'vldr', 'vstr'):
            c[mnemonic + '_instructions'] += counts[mnemonic]
            c[mnemonic + '_functions'] += bool(counts[mnemonic])
        # Narrow positive signature: adjacent MOVW/MOVT to the same register,
        # followed immediately by a PC-relative ADD/LDR using that register.
        # This is addressing-pattern evidence, never compiler attribution.
        address_pairs = 0
        for a, b, d in zip(instructions, instructions[1:], instructions[2:]):
            reg = a['operands'].split(',')[0]
            if a['instruction'] == 'movw' and b['instruction'] == 'movt' and b['operands'].split(',')[0] == reg:
                if ((d['instruction'] == 'add' and d['operands'] == reg + ', pc, ' + reg)
                        or (d['instruction'] == 'ldr' and d['operands'] == reg + ', [pc, ' + reg + ']')):
                    address_pairs += 1
        c['pc_relative_movw_movt_sequences'] += address_pairs
        c['pc_relative_movw_movt_functions'] += bool(address_pairs)
        # Require the branch to occupy the actual final bytes; no trimming pools.
        if instructions:
            last = instructions[-1]
            if (last['instruction'] in ('b', 'b.w') and
                    last['address'] + len(bytes.fromhex(last['bytes'])) == f['address'] + f['size']):
                match = re.fullmatch(r'#(0x[0-9a-f]+|[0-9]+)', last['operands'])
                dest = int(match[1], 0) if match else None
                if dest in targets and not f['address'] <= dest < f['address'] + f['size']:
                    c['terminal_external_b_functions'] += 1
        if f['symbol'] in EXAMPLES:
            result['examples'].append({**f, 'object_path': groups[f['group_id']]['object_path'],
                                       'bytes_sha256': hashlib.sha256(original.bytes_at(f['address'], f['size'], f['section'])).hexdigest(),
                                       'assembly': rows})
    totals = defaultdict(Counter)
    for gid, g in groups.items():
        kind = 'archive-member' if '.a(' in g['object_path'] else 'app-object'
        entry = {'id': gid, 'name': g['name'], 'kind': kind, 'object_path': g['object_path'],
                 'source_paths': g['source_paths'], 'modes': dict(Counter(f['mode'] for f in inventory['functions'] if f['group_id'] == gid)),
                 'object_timestamp': oso.get(g['object_path']), **dict(features[gid])}
        if entry['object_timestamp']:
            entry['object_timestamp_utc'] = datetime.fromtimestamp(entry['object_timestamp'], timezone.utc).isoformat()
        result['groups'].append(entry)
        totals[kind].update(features[gid])
    result['totals_by_kind'] = {k: dict(v) for k, v in totals.items()}
    return inventory, result


def run(cmd, out, root=ROOT):
    p = subprocess.run(cmd, cwd=root, text=True, capture_output=True,
                       errors='replace', timeout=120)
    out.write_text(p.stdout + p.stderr)
    return {'argv': cmd, 'returncode': p.returncode,
            'output_sha256': sha256(out), 'output': p.stdout + p.stderr}


def investigate(profile_path, input_path, output):
    output = output.resolve()
    relative = output.relative_to(ROOT)  # Container writes must be under /work.
    output.mkdir(parents=True, exist_ok=True)
    profile = load_json(profile_path)
    cmd = command(profile, ROOT)
    original = MachO(input_path.read_bytes())
    inventory, original_evidence = evidence(original)
    write_json(output / 'original.json', original_evidence)
    source = ROOT / 'toolchain/probes/codegen.cpp'
    # Also inspect the actual preprocessed TU using the existing source policy.
    prep = subprocess.run(cmd + ['-x', 'c++', '-E', str(source.relative_to(ROOT))],
                          cwd=ROOT, capture_output=True, text=True, timeout=120)
    if prep.returncode:
        raise RuntimeError('Probe preprocessing failed: ' + prep.stderr)
    validate_source(prep.stdout)
    metadata = {}
    for name, args in {'version': ['-v'], 'target': ['-dumpmachine'],
                       'search-dirs': ['-print-search-dirs'], 'specs': ['-dumpspecs'],
                       'cc1plus': ['-print-prog-name=cc1plus'],
                       'target-help': ['--help', '-v', '-x', 'c++', '-c', '/dev/null', '-o', '/dev/null'],
                       'llvm-options': ['-x', 'c++', '-march=armv7-a', '-S',
                                        '-mllvm', '-help', '/dev/null', '-o', '/dev/null']}.items():
        metadata[name] = run(cmd + args, output / (name + '.txt'))
    # Avoid absolute host paths in the durable command evidence.
    normalize = lambda args: [a.replace(str(ROOT), '<workspace>') for a in args]
    for record in metadata.values():
        record['argv'] = normalize(record['argv'])
    if profile.get('container'):
        paths = [cmd[-1], metadata['cc1plus']['output'].strip()]
        metadata['binary-hashes'] = run(cmd[:-1] + ['sha256sum'] + paths,
                                        output / 'binary-hashes.txt')
        metadata['backend-version'] = run(cmd[:-1] + [str(Path(cmd[-1]).parent / 'llc'), '--version'],
                                           output / 'backend-version.txt')
    records = []
    for case in matrix():
        name, flags = case['name'], case['flags']
        obj = output / (name + '.o')
        obj.unlink(missing_ok=True)
        invocation = cmd + ['-x', 'c++'] + flags
        args = ['-c', str(source.relative_to(ROOT)), '-o', str(relative / obj.name)]
        compile_result = run(invocation + args, output / (name + '.log'))
        record = {**case, 'command': normalize(invocation + args),
                  'returncode': compile_result['returncode'], 'diagnostics': compile_result['output']}
        if not compile_result['returncode']:
            record['object_sha256'] = sha256(obj)
            record['mach_o_header'] = dict(zip(('magic', 'cpu', 'subtype', 'filetype'),
                                               struct.unpack_from('<4I', obj.read_bytes())))
            try:
                candidate = MachO(obj.read_bytes())
            except ToolError as error:
                record['object_analysis_error'] = str(error)
                records.append(record)
                print(name + ': compiled outside ARMv7 reader support', flush=True)
                continue
            record['probes'] = {}
            for symbol in PROBES:
                span = candidate_span(candidate, symbol)
                if span is None:
                    record['probes'][symbol] = {'missing': True}
                    continue
                sym, size = span
                mode = 'thumb' if sym.thumb else 'arm'
                rows = assembly(candidate.bytes_at(sym.value & ~1, size, sym.section),
                                sym.value & ~1, mode)
                record['probes'][symbol] = {'mode': mode, 'size': size, 'assembly': rows}
            record['diagnostic_comparisons'] = []
            for function in inventory['functions']:
                if function['symbol'] in EXAMPLES:
                    compared = compare_function(original, inventory, function, candidate)
                    record['diagnostic_comparisons'].append({
                        'id': function['id'], 'symbol': function['symbol'],
                        **{k: compared.get(k) for k in ('status', 'byte_equal', 'candidate_size', 'reasons', 'relocations')}})
            # Record assembly independently of assembler/linker behavior.
            assembly_result = run(invocation + ['-S', str(source.relative_to(ROOT)),
                                    '-o', str(relative / (name + '.s'))], output / (name + '-assembly.log'))
            record['assembly_returncode'] = assembly_result['returncode']
            if not assembly_result['returncode']:
                record['assembly_sha256'] = sha256(output / (name + '.s'))
        records.append(record)
        print(name + ': ' + ('compiled' if not record['returncode'] else 'failed'), flush=True)
    # A repeated object protects against mistaking an unstable experiment for evidence.
    reference = next(r for r in records if r['name'] == 'arm-O2')
    ir = run(cmd + ['-x', 'c++'] + reference['flags'] + ['-S', '-emit-llvm',
                  str(source.relative_to(ROOT)), '-o', str(relative / 'arm-O2.ll')],
             output / 'arm-O2-ir.log')
    metadata['llvm-ir'] = {**ir, 'argv': normalize(ir['argv'])}
    if not ir['returncode']:
        metadata['llvm-ir']['ir_sha256'] = sha256(output / 'arm-O2.ll')
    # Only Darwin triples are candidates for this Mach-O toolchain. An ELF
    # target could exercise a different lowering path but would change the ABI.
    if profile.get('container') and not ir['returncode']:
        for triple in ('armv7-apple-darwin', 'armv7-apple-darwin10', 'armv7-apple-darwin11'):
            asm_path = output / (triple + '.s')
            triple_run = run(cmd[:-1] + [str(Path(cmd[-1]).parent / 'llc'), '-march=arm',
                              '-mcpu=cortex-a8', '-mtriple=' + triple, '-relocation-model=pic',
                              str(relative / 'arm-O2.ll'), '-o', str(relative / asm_path.name)],
                             output / (triple + '.log'))
            metadata[triple] = {**triple_run, 'argv': normalize(triple_run['argv'])}
            if not triple_run['returncode']:
                metadata[triple]['assembly_sha256'] = sha256(asm_path)
    repeat = output / 'arm-O2-repeat.o'
    repeat.unlink(missing_ok=True)
    repeated = run(cmd + ['-x', 'c++'] + reference['flags'] + ['-c', str(source.relative_to(ROOT)),
                   '-o', str(relative / repeat.name)], output / 'arm-O2-repeat.log')
    reproducible = not repeated['returncode'] and reference.get('object_sha256') == sha256(repeat)
    result = {'version': 1, 'profile': profile,
              'profile_file_sha256': sha256(profile_path), 'source_sha256': sha256(source),
              'script_sha256': sha256(Path(__file__)), 'metadata': metadata,
              'source_policy_validated': True, 'original_sha256': original_evidence['executable_sha256'],
              'reproducible_O2_object': reproducible, 'experiments': records,
              'matching_credit': False,
              'conclusion': 'Diagnostic evidence only. Compiler equivalence and original per-unit flags require independent provenance.'}
    write_json(output / 'report.json', result)
    print('Evidence: ' + str(output / 'report.json'))
    return result


if __name__ == '__main__':
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--profile', type=Path, default=ROOT / 'build/toolchain/compiler.json')
    p.add_argument('--input', type=Path, default=ROOT / 'research/Pirates-executable')
    p.add_argument('--output', type=Path, default=ROOT / 'build/compiler-investigation/current')
    args = p.parse_args()
    investigate(args.profile, args.input, args.output)
