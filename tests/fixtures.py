"""Small synthetic Mach-O writer; no proprietary bytes or SDK required."""
import struct
from pathlib import Path


def symbol(name, value=0, section=1, desc=0, type=0x0f):
    return (name, type, section, desc, value)


def executable_symbols(functions, extras=()):
    records = []
    groups = {}
    for name, address, size, mode, group in functions:
        groups.setdefault(group, []).append((name, address, size, mode))
    for group, funcs in groups.items():
        records += [('/synthetic/', 0x64, 0, 0, 0), (f'/synthetic/{group}.cpp', 0x64, 0, 0, 0),
                    (f'/synthetic/{group}.o', 0x66, 0, 0, 0)]
        for name, address, size, mode in funcs:
            records += [(name, 0x24, 1, 0, address), ('', 0x24, 0, 0, size)]
        records += [('', 0x64, 0, 0, 0)]
    records += [symbol(name, address, desc=8 if mode == 'thumb' else 0)
                for name, address, size, mode, group in functions]
    return records + list(extras)


def macho(sections, symbols, filetype=1, data_ranges=()):
    """sections: (segment, name, address, bytes, flags, [relocation dict])."""
    section_count = len(sections)
    segment_size = 56 + 68 * section_count
    commands_size = segment_size + 24 + (16 if data_ranges else 0)
    cursor = 28 + commands_size
    blobs, sec_records = [], []
    for segment, name, address, data, flags, relocations in sections:
        off = cursor
        blobs.append(data)
        cursor += len(data)
        roff = cursor
        for r in relocations:
            if r.get('scattered'):
                a = 0x80000000 | int(r.get('pcrel', False)) << 30 | r['length'] << 28 | r['type'] << 24 | r['address']
                b = r.get('value', 0)
            else:
                a = r['address']
                b = (r['type'] << 28 | int(r.get('external', False)) << 27 | r['length'] << 25
                     | int(r.get('pcrel', False)) << 24 | r.get('symbol', 0))
            blobs.append(struct.pack('<II', a, b))
            cursor += 8
        sec_records.append(struct.pack('<16s16s9I', name.encode(), segment.encode(), address, len(data), off, 2,
                                       roff, len(relocations), flags, 0, 0))
    symoff = cursor
    strings = bytearray(b'\0')
    for name, t, s, d, v in symbols:
        ix = len(strings) if name else 0
        if name:
            strings.extend(name.encode() + b'\0')
        blobs.append(struct.pack('<IBBHI', ix, t, s, d, v))
        cursor += 12
    stroff = cursor
    blobs.append(bytes(strings))
    cursor += len(strings)
    datacmd = b''
    if data_ranges:
        datacmd = struct.pack('<4I', 0x29, 16, cursor, 8 * len(data_ranges))
        for off, sz, kind in data_ranges:
            blobs.append(struct.pack('<IHH', off, sz, kind))
    segment = struct.pack('<II16s8I', 1, segment_size, b'', 0, 0, 0, 0, 7, 7, section_count, 0) + b''.join(sec_records)
    symcmd = struct.pack('<6I', 2, 24, symoff, len(symbols), stroff, len(strings))
    header = struct.pack('<7I', 0xfeedface, 12, 9, filetype, 3 if data_ranges else 2, commands_size, 0x2000)
    return header + segment + symcmd + datacmd + b''.join(blobs)


def text(data, address=0, relocations=()):
    return ('__TEXT', '__text', address, data, 0x80000400, relocations)


def reference(data, mode='arm', address=0x1000, name='_probe', extras=(), extra_sections=()):
    funcs = [(name, address, len(data), mode, 'unity')]
    return macho([text(data, address), *extra_sections], executable_symbols(funcs, extras), filetype=2)
