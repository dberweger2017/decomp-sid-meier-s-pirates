#!/usr/bin/env python3
"""Validate reviewed ABI-entry recovery and emit ordinary C++ forwarding calls.

Assembly is inspected only as input. GNU asm declarations below name external
symbols; they contain no instructions. Callee bodies and object layouts receive
no recovery credit from an entry wrapper.
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, load_json
import capstone

TYPES = {'void *', 'const void *', 'bool', 'char', 'signed char', 'unsigned char',
         'short', 'unsigned short', 'int', 'unsigned int', 'long', 'unsigned long', 'float'}
RESULTS = TYPES | {'void', 'double'}
KINDS = {'complete-destructor', 'complete-constructor', 'method-forwarder', 'function-forwarder'}
SYMBOL = re.compile(r'_[A-Za-z_][A-Za-z_0-9]*\Z')
ID = re.compile(r'f-[a-f0-9]{20}\Z')


def validate(specification, inventory, original):
    if specification.get('version') != 1 or not isinstance(specification.get('entries'), list):
        raise ToolError('ABI forwarder specification requires version 1 and entries')
    functions = {f['id']: f for f in inventory['functions']}
    seen = set()
    cs = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    cs.detail = True
    for entry in specification['entries']:
        fid = entry['id']
        if not ID.fullmatch(fid) or fid in seen or fid not in functions:
            raise ToolError('Unknown or repeated ABI entry ID: ' + fid)
        seen.add(fid)
        function = functions[fid]
        if entry['group_id'] != function['group_id'] or entry['symbol'] != function['symbol']:
            raise ToolError('ABI entry changed original inventory identity: ' + fid)
        if (function['mode'] != 'arm' or function['size'] != 16 or function['ambiguities']
                or entry['kind'] not in KINDS):
            raise ToolError('Unsupported or ambiguous ABI entry: ' + fid)
        for name in ('symbol', 'target_symbol'):
            if not SYMBOL.fullmatch(entry[name]):
                raise ToolError('Invalid ABI linkage symbol: ' + fid)
        parameters, target_parameters = entry['parameters'], entry['target_parameters']
        if (entry['return_type'] not in RESULTS or not isinstance(parameters, list)
                or not isinstance(target_parameters, list) or len(parameters) > 4
                or len(target_parameters) > len(parameters)
                or any(t not in TYPES for t in parameters + target_parameters)):
            raise ToolError('ABI entry requires known register-only parameters: ' + fid)
        # Restrict adapters to unchanged register arguments. Wider/by-value/stack
        # arguments and conversion/adjustment thunks need independent recovery.
        if parameters[:len(target_parameters)] != target_parameters:
            # Signed/unsigned 32-bit words have identical ARM register passing.
            words = {'int', 'unsigned int', 'long', 'unsigned long'}
            if any(a != b and not (a in words and b in words)
                   for a, b in zip(parameters, target_parameters)):
                raise ToolError('ABI entry changes argument representation: ' + fid)
        target_symbols = [s for s in original.symbols if s.defined and s.name == entry['target_symbol']]
        if not target_symbols or len({s.value for s in target_symbols}) != 1:
            raise ToolError('ABI entry has no unique named original target: ' + fid)
        target = target_symbols[0].value & ~1
        instructions = list(cs.disasm(original.bytes_at(function['address'], 16, function['section']), function['address']))
        if (len(instructions) != 4 or instructions[0].mnemonic != 'push'
                or instructions[0].op_str != '{r7, lr}' or instructions[1].mnemonic != 'mov'
                or instructions[1].op_str != 'r7, sp' or instructions[2].mnemonic != 'bl'
                or not instructions[2].operands or instructions[2].operands[0].type != capstone.arm.ARM_OP_IMM
                or instructions[2].operands[0].imm != target
                or instructions[3].mnemonic != 'pop' or instructions[3].op_str != '{r7, pc}'):
            raise ToolError('Original entry is not the reviewed direct forwarder: ' + fid)
        if entry['kind'] in {'complete-destructor', 'complete-constructor'}:
            before, after = ('D1E', 'D2E') if entry['kind'] == 'complete-destructor' else ('C1E', 'C2E')
            if (before not in entry['symbol'] or entry['symbol'].replace(before, after) != entry['target_symbol']
                    or not parameters or parameters[0] != 'void *' or parameters != target_parameters
                    or entry['return_type'] != 'void'):
                raise ToolError('Invalid complete/base lifetime-entry relationship: ' + fid)
            if before == 'D1E' and len(parameters) != 1:
                raise ToolError('Destructor entry must forward only this: ' + fid)
    return specification['entries']


def identifier(entry):
    return 'pirates_' + entry['kind'].replace('-', '_') + '_' + entry['id'][2:]


def render(entries, group_id):
    lines = ['// Original compilation group ' + group_id + '.',
             '// Recovered ARM ABI entry points, expressed as ordinary C++ calls.',
             '// Opaque pointer parameters describe register passing, not complete types.',
             '// GNU asm labels are symbol linkage only; no instruction/byte bodies.',
             '// Callee implementations, full layouts and unencoded results remain separate.', '']
    for entry in sorted(entries, key=lambda e: e['id']):
        name = identifier(entry)
        params = ', '.join(t + ' a' + str(i) for i, t in enumerate(entry['parameters'])) or 'void'
        target_params = ', '.join(entry['target_parameters']) or 'void'
        call = ', '.join('a' + str(i) for i in range(len(entry['target_parameters'])))
        prefix = '' if entry['return_type'] == 'void' else 'return '
        lines += ['// ' + entry['id'] + ' — ' + entry['kind'],
                  '// ' + entry['original_name'], '// Calls: ' + entry['target_name'],
                  'extern "C" ' + entry['return_type'] + ' ' + name + '_target(' + target_params + ')',
                  '    __asm__(' + json.dumps(entry['target_symbol']) + ');',
                  'extern "C" ' + entry['return_type'] + ' ' + name + '(' + params + ')',
                  '    __asm__(' + json.dumps(entry['symbol']) + ');',
                  'extern "C" ' + entry['return_type'] + ' ' + name + '(' + params + ') {',
                  '    ' + prefix + name + '_target(' + call + ');', '}', '']
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('specification', type=Path)
    parser.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument('--output', type=Path, help='Emit group sources under this directory after validation')
    args = parser.parse_args()
    root = args.workspace.resolve()
    config = load_json(root / 'build/config.json')
    entries = validate(load_json(args.specification), load_json(root / 'build/inventory.json'),
                       MachO((root / config['provenance']['input']).read_bytes()))
    groups = sorted({e['group_id'] for e in entries})
    if args.output:
        args.output.mkdir(parents=True, exist_ok=True)
        for group in groups:
            (args.output / (group + '.cpp')).write_text(render([e for e in entries if e['group_id'] == group], group))
    print(f'Validated {len(entries)} register-only ABI forwarders in {len(groups)} original groups')


if __name__ == '__main__':
    try:
        main()
    except (ToolError, OSError, ValueError, KeyError, TypeError) as error:
        sys.exit(str(error))
