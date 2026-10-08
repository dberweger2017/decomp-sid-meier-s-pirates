"""Non-code allocation inventory and relocation-resolved data comparison.

Mach-O nlist does not give C object sizes. Compare complete allocation spans
through the next named definition or section end, retaining padding. This is
allocation equality, not a claim about a recovered C type's sizeof.
"""
from collections import Counter, defaultdict
from .util import stable_id, ToolError
from .relocations import AddressResolver, relocate


def recover_data(macho, inventory):
    ownership = defaultdict(set)
    occurrences = Counter()
    group = None
    for symbol in macho.symbols:
        if symbol.type == 0x64 and not symbol.name:
            group = None
        elif symbol.type == 0x66 and symbol.name:
            occurrences[symbol.name] += 1
            group = 'o-' + stable_id(symbol.name, occurrences[symbol.name])
        elif group and symbol.type in (0x20, 0x26, 0x28) and symbol.name:
            # N_GSYM frequently has value/section zero; use its exact name.
            ownership[symbol.name].add(group)
            if symbol.section:
                ownership[(symbol.name, symbol.section, symbol.value)].add(group)
    records, sections = [], []
    for section in macho.sections:
        if section.code:
            continue  # Literal pools remain part of code and are not double counted.
        names = defaultdict(set)
        for symbol in macho.symbols:
            if symbol.defined and symbol.section == section.index and not symbol.name.startswith('L'):
                if not section.address <= symbol.value < section.address + section.size:
                    raise ToolError('Data definition outside section: ' + symbol.name)
                names[symbol.value].add(symbol.name)
        starts = sorted(set(names) | ({section.address} if section.size else set()))
        sections.append({'section': section.index, 'name': section.key, 'size': section.size,
                         'address': section.address, 'alignment': macho.alignments[section.index],
                         'zerofill': section.zerofill})
        for i, start in enumerate(starts):
            end = starts[i + 1] if i + 1 < len(starts) else section.address + section.size
            aliases = sorted(names[start])
            owners = set()
            for name in aliases:
                owners.update(ownership.get((name, section.index, start), ownership.get(name, set())))
            gid = next(iter(owners)) if len(owners) == 1 else 'unowned-data'
            ambiguity = []
            if len(owners) > 1:
                ambiguity.append('ambiguous original object ownership')
            if not aliases:
                ambiguity.append('unnamed section gap; no candidate symbol boundary')
            label = aliases[0] if aliases else section.key + ' gap'
            records.append({'id': 'd-' + stable_id(gid, section.key, '|'.join(aliases), start),
                            'group_id': gid, 'symbol': label, 'aliases': aliases, 'address': start,
                            'section': section.index, 'section_name': section.key, 'size': end - start,
                            'alignment': macho.alignments[section.index], 'zerofill': section.zerofill,
                            'boundary': 'whole allocation to next named definition or section end; includes padding',
                            'ownership': 'STABS' if len(owners) == 1 else 'unattributed',
                            'ambiguities': ambiguity})
    return {'version': 1, 'scope': 'all non-instruction section allocations, including padding and zero-fill',
            'sections': sections, 'records': records,
            'coverage': {'records': len(records), 'sections': len(sections),
                         'bytes': sum(s['size'] for s in sections),
                         'unattributed_bytes': sum(r['size'] for r in records if r['group_id'] == 'unowned-data')}}


def compare_data(original, inventory, record, candidate, mapping=None, placements=None, details=False):
    mapping, placements = mapping or {}, placements or {}
    result = {**record, 'status': 'missing', 'byte_equal': False, 'byte_verified': False,
              'similarity': None, 'candidate_size': None, 'reasons': [], 'relocations': [], 'rows': []}
    left, right = b'', b''
    if not record['zerofill'] and details:
        left = original.bytes_at(record['address'], min(record['size'], 4096), record['section'])
    try:
        if candidate:
            name = mapping.get('symbol', record['symbol'])
            symbols = [s for s in candidate.symbols if s.defined and s.section and s.name == name
                       and not candidate.section(s.section).code]
            if not symbols:
                return display(result, left, right, details)
            if len(symbols) != 1:
                raise ToolError('Ambiguous candidate data name: ' + name)
            symbol = symbols[0]
            section = candidate.section(symbol.section)
            if record['ambiguities']:
                raise ToolError('; '.join(record['ambiguities']))
            following = [s.value for s in candidate.symbols if s.defined and s.section == symbol.section
                         and s.value > symbol.value and not s.name.startswith('L')]
            size = min(following, default=section.address + section.size) - symbol.value
            if size <= 0 or symbol.value < section.address:
                raise ToolError('Invalid candidate data boundary')
            aliases = [s.name for s in candidate.symbols if s.defined and s.section == symbol.section
                       and s.value == symbol.value and s.name != name and not s.name.startswith('L')]
            if aliases:
                raise ToolError('Candidate data aliases require explicit boundary evidence')
            result['candidate_size'] = size
            if section.key != record['section_name'] or section.zerofill != record['zerofill']:
                raise ToolError('Candidate data section/storage class differs')
            if candidate.alignments[section.index] != record['alignment']:
                raise ToolError('Candidate section alignment differs')
            if symbol.value % record['alignment'] != record['address'] % record['alignment']:
                raise ToolError('Candidate allocation alignment offset differs')
            if size != record['size']:
                result.update(status='different', reasons=['Whole allocation size differs; padding is retained'])
            elif section.zerofill:
                if section.relocations:
                    raise ToolError('Relocations in zero-fill storage are unsupported')
                result.update(status='matched', byte_equal=True, byte_verified=True)
            else:
                begin = symbol.value - section.address
                if any(r.type in (5, 6, 8, 9) and begin <= r.address < begin + size for r in section.relocations):
                    raise ToolError('Instruction relocation in non-code data is unsupported')
                if size > 64 * 1024 * 1024:
                    raise ToolError('Data allocation exceeds comparison resource limit')
                resolver = AddressResolver(original, inventory, candidate, record['group_id'],
                                           {**record, 'mode': 'unknown'}, symbol,
                                           placements.get('symbols'), placements.get('sections'))
                right, events, errors = relocate(candidate, symbol, size, record['address'], resolver)
                result['relocations'] = events
                left = original.bytes_at(record['address'], size, record['section'])
                result['byte_equal'] = left == right
                if errors:
                    result.update(status='unresolved', reasons=errors)
                else:
                    result.update(status='matched' if left == right else 'different', byte_verified=left == right)
            if details and not section.zerofill and not right:
                right = candidate.bytes_at(symbol.value, min(size, 4096), symbol.section)
    except ToolError as error:
        result.update(status='unresolved', reasons=[str(error)])
    return display(result, left, right, details)


def display(result, left, right, details):
    if details:
        # Bounded display; the equality comparison above includes every byte.
        for offset in range(0, min(max(len(left), len(right)), 4096), 16):
            l, r = left[offset:offset + 16], right[offset:offset + 16]
            words = lambda data: ' '.join(f'{int.from_bytes(data[i:i + 4], "little"):08x}' for i in range(0, len(data) - 3, 4))
            ascii_text = lambda data: ''.join(chr(b) if 32 <= b < 127 else '.' for b in data)
            result['rows'].append({'offset': offset, 'original': l.hex(' '), 'candidate': r.hex(' '), 'different': l != r,
                                   'original_words': words(l), 'candidate_words': words(r),
                                   'original_ascii': ascii_text(l), 'candidate_ascii': ascii_text(r)})
        result['display_truncated'] = result['size'] > 4096
    else:
        result.pop('rows')
    return result
