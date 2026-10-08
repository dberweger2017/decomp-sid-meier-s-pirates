#!/usr/bin/env python3
"""Review ABI leaf bodies without pretending their complete class types are known.

Local callbacks retain local linkage. Bodies are ordinary C++, with asm labels
only for the original symbol name. Ignored arguments and unencoded result types
remain partial ABI hypotheses; full declarations are separate recovery work.
"""
import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import capstone
from tools.abi_forwarders import ID, SYMBOL, TYPES
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError, load_json


def validate(spec, inventory, original):
    if spec.get('version') != 1 or not isinstance(spec.get('entries'), list):
        raise ToolError('Tiny-entry specification requires version 1 and entries')
    functions = {f['id']: f for f in inventory['functions']}
    seen = set()
    cs = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    for e in spec['entries']:
        fid = e['id']
        if not ID.fullmatch(fid) or fid in seen or fid not in functions:
            raise ToolError('Unknown or repeated tiny-entry ID: ' + fid)
        seen.add(fid)
        f = functions[fid]
        if (f['group_id'] != e['group_id'] or f['symbol'] != e['symbol']
                or f['mode'] != 'arm' or f['ambiguities'] or not SYMBOL.fullmatch(e['symbol'])
                or not isinstance(e['original_name'], str) or any(c in e['original_name'] for c in '\r\n')):
            raise ToolError('Invalid or ambiguous tiny-entry identity: ' + fid)
        if (not isinstance(e['parameters'], list) or any(t not in TYPES for t in e['parameters'])
                or not isinstance(e['internal'], bool)):
            raise ToolError('Invalid partial ABI parameters: ' + fid)
        symbols = [s for s in original.symbols if s.defined and s.name == e['symbol']
                   and (s.value & ~1) == f['address']]
        if len(symbols) != 1 or e['internal'] != (not bool(symbols[0].type & 1)):
            raise ToolError('Tiny entry must preserve original local/external linkage: ' + fid)
        a = [(i.mnemonic, i.op_str) for i in cs.disasm(
            original.bytes_at(f['address'], f['size'], f['section']), f['address'])]
        kind, ret = e['kind'], e['return_type']
        if kind in {'empty-body', 'destruction-callback', 'empty-initializer'}:
            valid = a == [('bx', 'lr')] and ret == 'void'
            if kind == 'destruction-callback':
                valid &= e['symbol'].startswith('___tcf_') and e['parameters'] == ['void *'] and e['internal']
            if kind == 'empty-initializer':
                valid &= e['symbol'].startswith('__GLOBAL__I_') and not e['parameters'] and e['internal']
        elif kind == 'identity-body':
            valid = a == [('bx', 'lr')] and ret == 'void *' and e['parameters'] and e['parameters'][0] == 'void *'
        elif kind == 'constant-body':
            value = e['value']
            valid = (type(value) is int and value in (0, 1) and a == [('mov', 'r0, #' + str(value)), ('bx', 'lr')]
                     and ret in {'bool', 'unsigned int', 'int', 'void *', 'float'}
                     and (not value or ret not in {'float', 'void *'}))
        elif kind == 'adjusted-field-getter':
            offset = e['offset']
            valid = (type(offset) is int and 0 < offset <= 65535 and offset % 4 == 0
                     and a == [('ldr', 'r0, [r0, #0x' + format(offset, 'x') + ']'), ('bx', 'lr')]
                     and ret == 'unsigned int' and e['parameters'] == ['void *']
                     and e['symbol'].startswith('__ZThn'))
        else:
            valid = False
        if not valid:
            raise ToolError('Original bytes do not prove the reviewed leaf body: ' + fid)
    return spec['entries']


def render(entries, group):
    lines = ['// Original compilation group ' + group + '.',
             '// Partial ABI leaf bodies. Full types and unencoded results remain hypotheses.',
             '// No instruction or original-byte bodies; asm declarations name linkage only.', '']
    for e in sorted(entries, key=lambda e: e['id']):
        name = 'pirates_leaf_' + e['id'][2:]
        params = ', '.join(t + ' a' + str(i) for i, t in enumerate(e['parameters'])) or 'void'
        linkage = 'static ' if e['internal'] else 'extern "C" '
        body = ''
        if e['kind'] == 'identity-body':
            body = 'return a0;'
        elif e['kind'] == 'constant-body':
            body = 'return ' + str(e['value']) + ';'
        elif e['kind'] == 'adjusted-field-getter':
            view = name + '_subobject'
            lines += ['// This thunk sees the +180 render-unit subobject: +252 - 180 = +72.',
                      'struct ' + view + ' { unsigned char unknown[' + str(e['offset']) + ']; unsigned int vertex_count; };']
            body = 'return static_cast<' + view + ' *>(a0)->vertex_count;'
        lines += ['// ' + e['id'] + ' — ' + e['kind'], '// ' + e['original_name'],
                  linkage + e['return_type'] + ' ' + name + '(' + params + ')',
                  '    __asm__(' + json.dumps(e['symbol']) + ');',
                  linkage + e['return_type'] + ' ' + name + '(' + params + ') { ' + body + ' }', '']
        if e['internal']:
            # Preserve private source bodies in Mach-O objects without `used`,
            # which also prevents dead stripping on the historical backend.
            lines += ['// Source-emission reference only; no original data or lifetime-registration credit.',
                      'extern "C" ' + e['return_type'] + ' (* const ' + name + '_source_reference)(' +
                      (', '.join(e['parameters']) or 'void') + ') = ' + name + ';', '']
    return '\n'.join(lines)


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('specification', type=Path)
    p.add_argument('--workspace', type=Path, default=Path(__file__).resolve().parents[1])
    p.add_argument('--output', type=Path)
    p.add_argument('--check-source', action='store_true')
    args = p.parse_args()
    root = args.workspace.resolve()
    config = load_json(root / 'build/config.json')
    entries = validate(load_json(args.specification), load_json(root / 'build/inventory.json'),
                       MachO((root / config['provenance']['input']).read_bytes()))
    groups = sorted({e['group_id'] for e in entries})
    for group in groups:
        source = render([e for e in entries if e['group_id'] == group], group)
        if args.check_source and (root / 'src/recovery/leaves' / (group + '.cpp')).read_text() != source:
            raise ToolError('Tiny-entry source differs from reviewed specification: ' + group)
        if args.output:
            args.output.mkdir(parents=True, exist_ok=True)
            path = args.output / (group + '.cpp')
            if not path.exists() or path.read_text() != source:
                path.write_text(source)
    print(f'Validated {len(entries)} tiny ABI leaf bodies in {len(groups)} original groups')


if __name__ == '__main__':
    try:
        main()
    except (ToolError, OSError, ValueError, KeyError, TypeError) as error:
        sys.exit(str(error))
