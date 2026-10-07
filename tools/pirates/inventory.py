"""STABS records are the authoritative inventory, including uncertain records."""
from collections import Counter, defaultdict
from .util import stable_id


def recover(macho):
    groups, functions = [], []
    group = None
    source, directory = '', ''
    pending = None
    occurrences = Counter()
    definitions = defaultdict(list)
    for s in macho.symbols:
        if s.defined:
            definitions[(s.name, s.value & ~1)].append(s)

    def finish(size=None):
        nonlocal pending
        if pending is None:
            return
        pending['size'] = size
        if size is None or size <= 0:
            pending['ambiguities'].append('missing or zero STABS size')
        functions.append(pending)
        pending = None

    for s in macho.symbols:
        if s.type == 0x64:  # N_SO
            if not s.name:
                finish()
                group = None
            elif s.name.endswith('/'):
                directory = s.name
            else:
                source = s.name if s.name.startswith('/') else directory + s.name
        elif s.type == 0x66 and s.name:  # N_OSO (one original object, including unity units)
            finish()
            occurrences[s.name] += 1
            group = {'id': 'o-' + stable_id(s.name, occurrences[s.name]), 'object_path': s.name,
                     'name': s.name.rsplit('/', 1)[-1], 'source_path': source, 'source_paths': [source] if source else []}
            groups.append(group)
        elif s.type == 0x84 and s.name:  # N_SOL (included sources/headers)
            source = s.name if s.name.startswith('/') else directory + s.name
            if group and source not in group['source_paths']:
                group['source_paths'].append(source)
        elif s.type == 0x24:  # N_FUN
            if not s.name:
                finish(s.value)
                continue
            finish()
            symbol = s.name  # Darwin N_FUN names include ObjC colons and block names verbatim.
            address = s.value & ~1
            gid = group['id'] if group else 'unowned'
            defs = definitions[(symbol, address)]
            modes = {'thumb' if d.thumb or d.value & 1 else 'arm' for d in defs}
            mode = next(iter(modes)) if len(modes) == 1 else ('thumb' if s.desc & 8 or s.value & 1 else 'unknown')
            pending = {'id': 'f-' + stable_id(gid, symbol, address), 'group_id': gid, 'symbol': symbol,
                       'address': address, 'section': s.section, 'mode': mode, 'source_path': source,
                       'record_index': s.index, 'ambiguities': []}
            if group is None:
                pending['ambiguities'].append('no original object group')
            if mode == 'unknown':
                pending['ambiguities'].append('no unambiguous ARM/Thumb symbol metadata')
    finish()
    ids = Counter(f['id'] for f in functions)
    for f in functions:
        if ids[f['id']] > 1:
            f['id'] += '-r' + str(f['record_index'])
            f['ambiguities'].append('duplicate object/symbol/address record')
        try:
            sec = macho.section(f['section'])
            if f['mode'] == 'arm' and f['address'] % 4:
                f['ambiguities'].append('unaligned ARM function address')
            if f['mode'] == 'thumb' and f['size'] and f['size'] % 2:
                f['ambiguities'].append('unaligned Thumb function size')
            if not sec.code:
                f['ambiguities'].append('function outside instruction section')
            if f['size']:
                macho.bytes_at(f['address'], f['size'], f['section'])
        except ValueError:
            f['ambiguities'].append('function range outside section')
    ordered = sorted(functions, key=lambda f: (f['section'], f['address'], f['record_index']))
    # Sweep detects nesting as well as overlap with an immediate neighbour.
    active = []
    for f in ordered:
        active = [a for a in active if a['section'] == f['section'] and a['address'] + (a['size'] or 0) > f['address']]
        for a in active:
            if 'overlapping function records' not in a['ambiguities']:
                a['ambiguities'].append('overlapping function records')
            if 'overlapping function records' not in f['ambiguities']:
                f['ambiguities'].append('overlapping function records')
        active.append(f)
    for g in groups:
        g['source_paths'].sort()
    return {'version': 1, 'architecture': 'armv7', 'groups': groups, 'functions': ordered,
            'sections': macho.section_metadata(), 'coverage': {'named_stabs_records': len(functions),
            'original_object_groups': len(groups), 'ambiguous_records': sum(bool(f['ambiguities']) for f in functions)}}
