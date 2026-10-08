"""Conservative Mach-O ARM relocation application; no address masking.

The supported subset is documented in docs/matching.md. Unknown encodings,
interworking transformations, veneers and unproven placement remain unresolved.
"""
import struct
from .util import ToolError


class Unresolved(ToolError):
    pass


def signed(value, bits):
    value &= (1 << bits) - 1
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def half_immediate(word, thumb):
    if thumb:
        return ((word & 15) << 12) | (((word >> 10) & 1) << 11) | (((word >> 28) & 7) << 8) | ((word >> 16) & 255)
    return ((word >> 16) & 15) << 12 | (word & 0xfff)


def encode_half(word, value, thumb, upper):
    if thumb:
        if word & 0x8000fbf0 != (0xf2c0 if upper else 0xf240):
            raise Unresolved('HALF does not point at the expected Thumb MOVW/MOVT')
        return ((word & ~0x70ff040f) | ((value >> 12) & 15) | (((value >> 11) & 1) << 10)
                | (((value >> 8) & 7) << 28) | ((value & 255) << 16))
    if word & 0x0ff00000 != (0x03400000 if upper else 0x03000000):
        raise Unresolved('HALF does not point at the expected ARM MOVW/MOVT')
    return (word & ~0x000f0fff) | ((value >> 12) << 16) | (value & 0xfff)


def thumb_displacement(word):
    s, j1, j2 = (word >> 10) & 1, (word >> 29) & 1, (word >> 27) & 1
    return signed((s << 24) | ((1 ^ j1 ^ s) << 23) | ((1 ^ j2 ^ s) << 22)
                  | ((word & 0x3ff) << 12) | (((word >> 16) & 0x7ff) << 1), 25)


def encode_thumb_branch(word, delta):
    # B.W and BL only. BLX needs alignment/interworking semantics and is unresolved.
    if word & 0xf800 != 0xf000 or word >> 16 & 0xd000 not in (0x9000, 0xd000):
        raise Unresolved('Unsupported Thumb branch encoding/interworking')
    if delta & 1 or not -(1 << 24) <= delta < 1 << 24:
        raise Unresolved('Thumb branch requires a veneer or has unaligned target')
    v = delta & 0x1ffffff
    s, i1, i2 = v >> 24, (v >> 23) & 1, (v >> 22) & 1
    return ((word & ~0x2fff07ff) | (s << 10) | ((v >> 12) & 0x3ff)
            | ((1 ^ i1 ^ s) << 29) | ((1 ^ i2 ^ s) << 27) | (((v >> 1) & 0x7ff) << 16))


class AddressResolver:
    def __init__(self, original, inventory, candidate, group_id, function, candidate_symbol,
                 symbol_addresses=None, section_addresses=None):
        self.candidate, self.function, self.candidate_symbol = candidate, function, candidate_symbol
        self.section_addresses = section_addresses or {}
        self.global_names = {}
        self.group_names = {}
        for s in original.symbols:
            if s.defined:
                self.global_names.setdefault(s.name, set()).add((s.value & ~1 if s.thumb else s.value, s.thumb))
        for f in inventory['functions']:
            table = self.group_names if f['group_id'] == group_id else self.global_names
            if f['mode'] != 'unknown':
                table.setdefault(f['symbol'], set()).add((f['address'], f['mode'] == 'thumb'))
        # Local static data names can repeat across original compilation units.
        # STABS ownership is stronger evidence than a global name lookup.
        if not hasattr(original, '_data_ownership'):
            from .data import recover_data
            original._data_ownership = recover_data(original, inventory)['records']
        for data in original._data_ownership:
            if data['group_id'] == group_id and not data['ambiguities']:
                for name in data['aliases']:
                    self.group_names.setdefault(name, set()).add((data['address'], False))
        self.explicit = symbol_addresses or {}

    def name(self, name, pointer=False):
        if name in self.explicit:
            value = self.explicit[name]
            known = self.group_names.get(name) or self.global_names.get(name, set())
            if known and value not in {address | int(pointer and thumb) for address, thumb in known}:
                raise Unresolved('Explicit placement contradicts the original symbol address: ' + name)
            return value, name
        values = self.group_names.get(name) or self.global_names.get(name, set())
        if len(values) != 1:
            raise Unresolved('Original symbol address is absent or ambiguous: ' + name)
        value, thumb = next(iter(values))
        return value | int(pointer and thumb), name

    def address(self, address, section_index=None, pointer=False):
        # The currently compared function has explicit, stable placement.
        f, s = self.function, self.candidate_symbol
        begin = s.value & ~1 if s.thumb else s.value
        low = address & ~1 if pointer and s.thumb else address
        if (section_index is None or section_index == s.section) and begin <= low < begin + (f['size'] or 0):
            return (f['address'] + low - begin) | (address & 1 if pointer and s.thumb else 0), f['symbol'] + f'+0x{low - begin:x}'
        # Only a known Thumb function pointer has a tag bit. Odd data addresses
        # are real offsets, and must never alias the preceding byte or symbol.
        syms = [s for s in self.candidate.symbols if s.defined
                and (s.value & ~1 if s.thumb else s.value) == (address & ~1 if pointer and s.thumb else address)
                and (section_index is None or s.section == section_index)]
        resolved = []
        for symbol in syms:
            try:
                value, label = self.name(symbol.name, pointer)
                resolved.append((value, label))
            except Unresolved:
                pass
        if len({x[0] for x in resolved}) == 1:
            return resolved[0]
        secs = [s for s in self.candidate.sections if (section_index is None or section_index == s.index)
                and s.address <= address < s.address + s.size and s.key in self.section_addresses]
        if len(secs) == 1:
            section = secs[0]
            return self.section_addresses[section.key] + address - section.address, section.key + f'+0x{address-section.address:x}'
        raise Unresolved(f'Unproven candidate address placement: 0x{address:x}')

    def branch_mode(self, address):
        modes = {thumb for values in self.global_names.values() for value, thumb in values if value == address}
        f = self.function
        if f['address'] <= address < f['address'] + (f['size'] or 0) and f['mode'] != 'unknown':
            modes.add(f['mode'] == 'thumb')
        if len(modes) != 1:
            raise Unresolved('Branch target ARM/Thumb mode is absent or ambiguous')
        return next(iter(modes))

    def target(self, reloc, encoded, pointer=False):
        if reloc.scattered:
            base, label = self.address(reloc.value, pointer=pointer)
            return base + signed(encoded - reloc.value, 32), label
        if reloc.external:
            symbol = self.candidate.symbols[reloc.symbol]
            base, label = self.name(symbol.name, pointer)
            # Mach-O external relocations encode a symbolic addend. Definitions
            # carrying N_ARM_THUMB_DEF may also carry the low Thumb bit.
            if symbol.defined and symbol.thumb:
                encoded &= ~1
            return base + signed(encoded, 32), label
        if reloc.symbol == 0:  # R_ABS: no adjustment
            return encoded, 'absolute'
        return self.address(encoded, reloc.symbol, pointer)


def relocate(macho, symbol, size, original_address, resolver):
    section = macho.section(symbol.section)
    address = symbol.value & ~1 if symbol.thumb else symbol.value
    begin = address - section.address
    data = bytearray(macho.bytes_at(address, size, symbol.section))
    events, errors, occupied = [], [], set()
    relocs = section.relocations
    i = 0
    while i < len(relocs):
        r = relocs[i]
        pair = None
        if r.type in (2, 3, 8, 9):
            if i + 1 < len(relocs) and relocs[i + 1].type == 1:
                pair = relocs[i + 1]
                i += 1
        i += 1
        width = 4 if r.type in (8, 9) else 1 << r.length
        offset = r.address - begin
        if r.type == 1:  # Orphan pair is malformed even without an offset.
            errors.append('Orphan ARM_RELOC_PAIR')
            continue
        if offset >= size or offset + width <= 0:
            continue
        event = {'offset': offset, 'type': r.type, 'length': r.length, 'pcrel': r.pcrel,
                 'external': r.external, 'scattered': r.scattered}
        try:
            if offset < 0 or offset + width > size:
                raise Unresolved('Relocation crosses function boundary')
            if width != 4:
                raise Unresolved('Only 32-bit relocation sites are supported')
            if any(p in occupied for p in range(offset, offset + width)):
                raise Unresolved('Overlapping relocation sites')
            occupied.update(range(offset, offset + width))
            word = struct.unpack_from('<I', data, offset)[0]
            old_pc = section.address + r.address
            new_pc = original_address + offset
            if r.type == 0:
                if r.pcrel:
                    raise Unresolved('PC-relative VANILLA is not supported')
                target, label = resolver.target(r, word, pointer=True)
                patched = target & 0xffffffff
            elif r.type in (2, 3):
                if not r.scattered or not pair or not pair.scattered or r.pcrel:
                    raise Unresolved('SECTDIFF requires a scattered target and subtractor pair')
                target, label = resolver.address(r.value)
                subtract, sublabel = resolver.address(pair.value)
                addend = signed(word - (r.value - pair.value), 32)
                patched = (target - subtract + addend) & 0xffffffff
                event.update(subtract_target=sublabel, subtract_address=subtract)
            elif r.type in (5, 6):
                if not r.pcrel or r.length != 2:
                    raise Unresolved('Branch relocation has invalid flags')
                if r.type == 5:
                    if word & 0x0e000000 != 0x0a000000 or word & 0xf0000000 == 0xf0000000:
                        raise Unresolved('Unsupported ARM branch encoding/interworking')
                    encoded = old_pc + 8 + signed((word & 0xffffff) << 2, 26)
                    target, label = resolver.target(r, encoded)
                    if resolver.branch_mode(target):
                        raise Unresolved('ARM branch to Thumb target requires interworking')
                    delta = target - new_pc - 8
                    if delta % 4 or not -(1 << 25) <= delta < 1 << 25:
                        raise Unresolved('ARM branch requires a veneer or has unaligned target')
                    patched = word & 0xff000000 | ((delta >> 2) & 0xffffff)
                else:
                    encoded = old_pc + 4 + thumb_displacement(word)
                    target, label = resolver.target(r, encoded)
                    if not resolver.branch_mode(target):
                        raise Unresolved('Thumb branch to ARM target requires interworking')
                    patched = encode_thumb_branch(word, target - new_pc - 4)
            elif r.type in (8, 9):
                if not pair or r.pcrel or pair.pcrel or pair.external:
                    raise Unresolved('HALF relocation requires a following PAIR and absolute flags')
                thumb, upper = bool(r.length & 2), bool(r.length & 1)
                if thumb != symbol.thumb:
                    raise Unresolved('HALF relocation ARM/Thumb mode differs from function')
                this = half_immediate(word, thumb)
                other = pair.address & 0xffff
                encoded = this << 16 | other if upper else other << 16 | this
                if r.type == 9:
                    if not r.scattered or not pair.scattered:
                        raise Unresolved('HALF_SECTDIFF requires scattered target/subtractor')
                    target, label = resolver.address(r.value)
                    subtract, sublabel = resolver.address(pair.value)
                    value = target - subtract + signed(encoded - (r.value - pair.value), 32)
                    event.update(subtract_target=sublabel, subtract_address=subtract)
                else:
                    value, label = resolver.target(r, encoded, pointer=True)
                    target = value
                patched = encode_half(word, (value >> 16 if upper else value) & 0xffff, thumb, upper)
            else:
                raise Unresolved('Unsupported ARM relocation type: ' + str(r.type))
            struct.pack_into('<I', data, offset, patched)
            event.update(target=label, target_address=target, before=f'{word:08x}', after=f'{patched:08x}', status='resolved')
        except (Unresolved, ToolError) as e:
            event.update(status='unresolved', reason=str(e))
            errors.append(str(e))
        events.append(event)
    return bytes(data), events, errors
