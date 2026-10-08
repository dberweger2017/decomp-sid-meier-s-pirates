"""Bounds-checked reader for thin, little-endian ARMv7 Mach-O (no native tools)."""
from dataclasses import dataclass, field, asdict
import struct
from .util import ToolError


@dataclass
class Symbol:
    index: int
    name: str
    type: int
    section: int
    desc: int
    value: int

    @property
    def defined(self):
        return not self.type & 0xe0 and self.type & 0x0e in (0x0e, 0x02)

    @property
    def thumb(self):
        return bool(self.desc & 8)


@dataclass
class Relocation:
    address: int
    type: int
    length: int
    pcrel: bool
    external: bool = False
    symbol: int = 0
    scattered: bool = False
    value: int = 0


@dataclass
class Section:
    index: int
    name: str
    segment: str
    address: int
    size: int
    offset: int
    flags: int
    relocations: list = field(default_factory=list)

    @property
    def key(self):
        return self.segment + ',' + self.name

    @property
    def code(self):
        return bool(self.flags & 0x80000400)

    @property
    def zerofill(self):
        return self.flags & 0xff in (1, 0x0c, 0x12)


class MachO:
    def __init__(self, data):
        self.data = data
        self.sections = []
        self.symbols = []
        self.data_ranges = []
        self.cryptid = 0
        self.uuid = None
        # Keep alignment separately so the locked function inventory is unchanged.
        self.alignments = {}
        self.imported_stubs = []
        indirect_sections = {}
        magic, cpu, subtype, self.filetype, count, cmdbytes, self.flags = self.unpack('<7I', 0)
        if magic != 0xfeedface or cpu != 12 or subtype & 0xffffff != 9:
            raise ToolError('Expected a thin little-endian 32-bit ARMv7 Mach-O')
        if self.filetype not in (1, 2):
            raise ToolError('Expected Mach-O object or executable')
        self.check(28, cmdbytes)
        if count > cmdbytes // 8:
            raise ToolError('Load command count exceeds command region')
        symtab = dysymtab = None
        pos, end = 28, 28 + cmdbytes
        for _ in range(count):
            cmd, size = self.unpack('<II', pos)
            if size < 8 or size % 4 or pos + size > end:
                raise ToolError('Malformed load command size')
            if cmd == 1:  # LC_SEGMENT
                if size < 56:
                    raise ToolError('Truncated segment')
                nsects = self.unpack('<I', pos + 48)[0]
                if 56 + 68 * nsects > size:
                    raise ToolError('Truncated section table')
                for j in range(nsects):
                    p = pos + 56 + 68 * j
                    name, seg, addr, sz, off, align, roff, nr, flags, r1, r2 = self.unpack('<16s16s9I', p)
                    section = Section(len(self.sections) + 1, self.name(name), self.name(seg), addr, sz, off, flags)
                    self.alignments[section.index] = 1 << align
                    if addr + sz > 0x100000000 or align > 31:
                        raise ToolError('Invalid section address/alignment')
                    if not section.zerofill:
                        self.check(off, sz)
                    self.check(roff, 8 * nr)
                    for k in range(nr):
                        a, b = self.unpack('<II', roff + 8 * k)
                        if a & 0x80000000:
                            rel = Relocation(a & 0xffffff, (a >> 24) & 15, (a >> 28) & 3,
                                             bool(a & 0x40000000), scattered=True, value=b)
                        else:
                            rel = Relocation(a, b >> 28, (b >> 25) & 3, bool(b & 0x1000000),
                                             bool(b & 0x8000000), b & 0xffffff)
                        # PAIR's address stores the other half, not a section offset.
                        if rel.type != 1 and rel.address + (4 if rel.type in (8, 9) else 1 << rel.length) > sz:
                            raise ToolError('Relocation outside section')
                        section.relocations.append(rel)
                    self.sections.append(section)
                    if flags & 0xff in (6, 7, 8):
                        indirect_sections[section.index] = (r1, r2)
            elif cmd == 2:
                if symtab is not None or size != 24:
                    raise ToolError('Malformed/duplicate symbol table command')
                symtab = self.unpack('<4I', pos + 8)
            elif cmd == 11:  # LC_DYSYMTAB
                if dysymtab is not None or size != 80:
                    raise ToolError('Malformed/duplicate dynamic symbol table command')
                dysymtab = self.unpack('<18I', pos + 8)
            elif cmd == 0x21:
                if size < 20:
                    raise ToolError('Truncated encryption info')
                cryptoff, cryptsize, self.cryptid = self.unpack('<3I', pos + 8)
                self.check(cryptoff, cryptsize)
            elif cmd == 0x1b:
                if size != 24:
                    raise ToolError('Malformed UUID command')
                self.uuid = self.data[pos + 8:pos + 24].hex()
            elif cmd == 0x29:  # LC_DATA_IN_CODE, offsets from Mach-O header
                if size != 16:
                    raise ToolError('Malformed data-in-code command')
                off, sz = self.unpack('<II', pos + 8)
                self.check(off, sz)
                if sz % 8:
                    raise ToolError('Malformed data-in-code entries')
                for p in range(off, off + sz, 8):
                    offset, length, kind = self.unpack('<IHH', p)
                    self.check(offset, length)
                    self.data_ranges.append((offset, length, kind))
            pos += size
        if pos != end:
            raise ToolError('Load command region does not match count')
        if self.cryptid:
            raise ToolError('Encrypted executable cannot be compared (cryptid != 0)')
        if symtab:
            off, n, strings, sz = symtab
            self.check(off, n * 12)
            self.check(strings, sz)
            for i in range(n):
                ix, t, s, d, v = self.unpack('<IBBHI', off + 12 * i)
                if ix >= sz:
                    raise ToolError('Symbol string index outside table')
                finish = self.data.find(b'\0', strings + ix, strings + sz)
                if finish < 0:
                    raise ToolError('Unterminated symbol name')
                name = self.name(self.data[strings + ix:finish])
                if s > len(self.sections) and not t & 0xe0:
                    raise ToolError('Invalid symbol section index')
                self.symbols.append(Symbol(i, name, t, s, d, v))
        if dysymtab is not None:
            off, n = dysymtab[12:14]
            self.check(off, n * 4)
            indirect = self.unpack('<' + str(n) + 'I', off)
            for index, (start, stride) in indirect_sections.items():
                section = self.section(index)
                width = stride if section.flags & 0xff == 8 else 4
                if not width or section.size % width or start + section.size // width > n:
                    raise ToolError('Invalid indirect symbol section range/stride')
                for symbol_index in indirect[start:start + section.size // width]:
                    if not symbol_index & 0xc0000000 and symbol_index >= len(self.symbols):
                        raise ToolError('Invalid indirect symbol index')
            # Only the observed 12-byte ARM absolute stub is supported. Verify
            # its pointer slot's indirect name as well; arbitrary executable
            # bytes, Thumb stubs and other linker encodings prove no address.
            if self.filetype == 2:
                for section in self.sections:
                    if section.flags & 0xff != 8:
                        continue
                    start, width = indirect_sections[section.index]
                    if width != 12:
                        continue
                    for i in range(section.size // width):
                        ix = indirect[start + i]
                        if ix & 0xc0000000:
                            continue
                        symbol = self.symbols[ix]
                        if symbol.type & 0xe0 or symbol.type & 0x0e != 0 or symbol.section or symbol.value or not symbol.name:
                            continue
                        address = section.address + i * width
                        first, second, pointer = self.unpack('<3I', section.offset + i * width)
                        if address % 4 or (first, second) != (0xe59fc000, 0xe59cf000):
                            continue
                        slots = [s for s in self.sections if s.flags & 0xff == 7 and
                                 s.address <= pointer and pointer + 4 <= s.address + s.size]
                        if len(slots) != 1 or (pointer - slots[0].address) % 4:
                            continue
                        slot = slots[0]
                        slot_start = indirect_sections[slot.index][0]
                        if indirect[slot_start + (pointer - slot.address) // 4] == ix:
                            self.imported_stubs.append({'symbol': symbol.name, 'address': address, 'mode': 'arm'})
        for section in self.sections:
            for r in section.relocations:
                if r.type == 1 or r.scattered:
                    continue
                if r.external and r.symbol >= len(self.symbols):
                    raise ToolError('Invalid relocation symbol index')
                if not r.external and r.symbol > len(self.sections):
                    raise ToolError('Invalid relocation section index')

    @staticmethod
    def name(data):
        try:
            return data.split(b'\0', 1)[0].decode('utf-8')
        except UnicodeDecodeError as e:
            raise ToolError('Invalid UTF-8 name') from e

    def check(self, off, size):
        if off < 0 or size < 0 or off + size > len(self.data):
            raise ToolError('Mach-O range outside file')

    def unpack(self, fmt, off):
        self.check(off, struct.calcsize(fmt))
        return struct.unpack_from(fmt, self.data, off)

    def section(self, index):
        if not 1 <= index <= len(self.sections):
            raise ToolError('Invalid section index')
        return self.sections[index - 1]

    def bytes_at(self, address, size, section=None):
        choices = [s for s in self.sections if (section is None or s.index == section)
                   and not s.zerofill and s.address <= address and address + size <= s.address + s.size]
        if len(choices) != 1:
            raise ToolError('Ambiguous or unmapped byte range')
        s = choices[0]
        return self.data[s.offset + address - s.address:s.offset + address - s.address + size]

    def section_metadata(self):
        return [{k: v for k, v in asdict(s).items() if k != 'relocations'} for s in self.sections]
