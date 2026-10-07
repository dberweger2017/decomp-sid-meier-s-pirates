import difflib
from collections import defaultdict
from capstone import Cs, CS_ARCH_ARM, CS_MODE_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN
from capstone.arm import ARM_OP_MEM, ARM_REG_PC, ARM_OP_REG
from .relocations import AddressResolver, relocate
from .util import ToolError


def assembly(data, address, mode, data_ranges=()):
    """Keep every byte visible, including invalid encodings and literal pools.

    PC-relative LDR literal detection is a display heuristic, not match evidence.
    LC_DATA_IN_CODE annotations override the heuristic. Equality always includes
    the original bytes, including pools, padding, and undecodable bytes.
    """
    if mode not in ('arm', 'thumb'):
        return [{'address': address + i, 'bytes': data[i:i+1].hex(), 'instruction': '.byte',
                 'operands': f'0x{data[i]:02x}', 'registers': [], 'kind': 'unknown'} for i in range(len(data))]
    cs = Cs(CS_ARCH_ARM, (CS_MODE_THUMB if mode == 'thumb' else CS_MODE_ARM) | CS_MODE_LITTLE_ENDIAN)
    cs.detail = True
    pools = {a: n for a, n in data_ranges if address <= a and a + n <= address + len(data)}
    # In a first pass recover literal loads. Do not omit or normalize addresses.
    for ins in cs.disasm(data, address):
        if ins.mnemonic.startswith('ldr'):
            for op in ins.operands:
                if op.type == ARM_OP_MEM and op.mem.base == ARM_REG_PC and op.mem.index == 0:
                    pc = (ins.address + 4) & ~3 if mode == 'thumb' else ins.address + 8
                    target = pc + op.mem.disp
                    if address <= target and target + 4 <= address + len(data):
                        pools.setdefault(target, 4)
    rows, offset = [], 0
    while offset < len(data):
        here = address + offset
        if here in pools:
            width = min(pools[here], len(data) - offset)
            raw = data[offset:offset + width]
            rows.append({'address': here, 'bytes': raw.hex(), 'instruction': '.word' if width == 4 else '.data',
                         'operands': f'0x{int.from_bytes(raw, "little"):0{width*2}x}', 'registers': [], 'kind': 'literal'})
        else:
            # Avoid disassembling across a declared data region.
            next_pool = min((a for a in pools if a > here), default=address + len(data))
            ins = next(cs.disasm(data[offset:next_pool - address], here, count=1), None)
            if ins:
                width = ins.size
                registers = sorted({ins.reg_name(op.reg) for op in ins.operands if op.type == ARM_OP_REG}
                                   | {ins.reg_name(r) for op in ins.operands if op.type == ARM_OP_MEM
                                      for r in (op.mem.base, op.mem.index) if r})
                rows.append({'address': here, 'bytes': ins.bytes.hex(), 'instruction': ins.mnemonic,
                             'operands': ins.op_str, 'registers': registers, 'kind': 'instruction'})
            else:
                width = min(2 if mode == 'thumb' else 4, next_pool - here, len(data) - offset)
                raw = data[offset:offset + width]
                rows.append({'address': here, 'bytes': raw.hex(), 'instruction': '.byte',
                             'operands': raw.hex(), 'registers': [], 'kind': 'undecodable'})
        offset += width
    return rows


def aligned_diff(left, right):
    # Align instructions/operands without address masking. Highlight bytes and
    # registers independently even when the mnemonic remains unchanged.
    key = lambda row: (row['instruction'], row['operands'])
    seq = difflib.SequenceMatcher(None, list(map(key, left)), list(map(key, right)), autojunk=False)
    pairs = []
    for tag, a, b, c, d in seq.get_opcodes():
        for i in range(max(b - a, d - c)):
            l = left[a + i] if a + i < b else None
            r = right[c + i] if c + i < d else None
            changes = [k for k in ('bytes', 'instruction', 'operands', 'registers', 'kind')
                       if l is None or r is None or l[k] != r[k]]
            pairs.append({'original': l, 'candidate': r, 'differences': changes})
    return pairs, round(seq.ratio() * 100, 4)


def candidate_span(macho, name, value=None):
    choices = [s for s in macho.symbols if s.name == name and s.defined
               and s.section and macho.section(s.section).code and (value is None or s.value == value)]
    if not choices:
        return None
    if len(choices) != 1:
        raise ToolError('Ambiguous candidate symbol; supply candidate_address: ' + name)
    symbol = choices[0]
    section = macho.section(symbol.section)
    begin = symbol.value & ~1
    # Darwin labels beginning with L are assembler-local labels, not functions.
    starts = [s.value & ~1 for s in macho.symbols if s.defined and s.section == symbol.section
              and not s.name.startswith('L') and s.value & ~1 > begin]
    end = min(starts, default=section.address + section.size)
    aliases = [s.name for s in macho.symbols if s.defined and s.section == symbol.section
               and s.value & ~1 == begin and s.name != symbol.name and not s.name.startswith('L')]
    if aliases:
        raise ToolError('Candidate boundary has aliases: ' + ', '.join(aliases))
    if begin < section.address or end <= begin:
        raise ToolError('Invalid candidate function boundary')
    return symbol, end - begin


def compare_function(original, inventory, function, candidate, mapping=None, placements=None, details=False):
    mapping, placements = mapping or {}, placements or {}
    result = {'id': function['id'], 'status': 'missing', 'byte_equal': False, 'similarity': None,
              'candidate_size': None, 'relocations': [], 'reasons': [], 'candidate_symbol': mapping.get('symbol', function['symbol'])}
    left = b''
    try:
        if not function['size']:
            raise ToolError('Original STABS boundary has no size')
        left = original.bytes_at(function['address'], function['size'], function['section'])
    except ToolError as e:
        result['reasons'].append(str(e))
    right = b''
    candidate_ranges = []
    candidate_mode = function['mode']
    if candidate is not None:
        try:
            span = candidate_span(candidate, result['candidate_symbol'], mapping.get('candidate_address'))
            if span:
                symbol, size = span
                result['candidate_size'] = size
                if size > 1024 * 1024:
                    raise ToolError('Candidate function exceeds comparison size limit')
                resolver = AddressResolver(original, inventory, candidate, function['group_id'], function, symbol,
                                           placements.get('symbols'), placements.get('sections'))
                right, events, errors = relocate(candidate, symbol, size, function['address'], resolver)
                section = candidate.section(symbol.section)
                file_start = section.offset + (symbol.value & ~1) - section.address
                candidate_ranges = [(function['address'] + off - file_start, length)
                                    for off, length, kind in candidate.data_ranges
                                    if file_start <= off and off + length <= file_start + size]
                result.update(relocations=events, reasons=result['reasons'] + errors)
                result['reasons'].extend(function['ambiguities'])
                mode = 'thumb' if symbol.thumb or symbol.value & 1 else 'arm'
                candidate_mode = mode
                if mode != function['mode']:
                    result['reasons'].append('Candidate ARM/Thumb mode differs from original')
                result['byte_equal'] = bool(left) and left == right
                result['status'] = 'unresolved' if result['reasons'] else ('matched' if result['byte_equal'] else 'different')
        except ToolError as e:
            result.update(status='unresolved', reasons=result['reasons'] + [str(e)])
    if right or details:
        original_ranges = []
        if function['section']:
            section = original.section(function['section'])
            file_start = section.offset + function['address'] - section.address
            original_ranges = [(function['address'] + off - file_start, length)
                               for off, length, kind in original.data_ranges
                               if file_start <= off and off + length <= file_start + len(left)]
        lrows = assembly(left, function['address'], function['mode'], original_ranges)
        rrows = assembly(right, function['address'], candidate_mode, candidate_ranges)
        rows, similarity = aligned_diff(lrows, rrows)
        result['similarity'] = similarity if right else None
        if details:
            result['rows'] = rows
    return result
