"""Pinned linker identity, structural image inspection and incremental link state.

Structural linking is not runtime validation or image equality. Completion is
credited only for a source-only replacement with full exact coverage and a
byte-identical original image. Diagnostic subset links always have zero credit.
"""
import hashlib
import json
import os
import re
import struct
import subprocess
from pathlib import Path
from .macho import MachO
from .sdk import inspect_sdk
from .util import load_json, write_json, sha256, ToolError, local_path
from .source_policy import SOURCE_POLICY


def digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(',', ':')).encode()).hexdigest()


def version_number(value):
    if not isinstance(value, str) or not re.fullmatch(r'\d+\.\d+(?:\.\d+)?', value):
        raise ToolError('Versions must be major.minor[.patch]')
    fields = list(map(int, value.split('.')))
    fields += [0] * (3 - len(fields))
    if fields[0] > 65535 or fields[1] > 255 or fields[2] > 255:
        raise ToolError('Mach-O version component out of range')
    return fields[0] << 16 | fields[1] << 8 | fields[2]


def reproduce_uuid(data, expected):
    """Reproduce a declared identity field; never mask code/data addresses."""
    if not re.fullmatch(r'[0-9a-f]{32}', expected):
        raise ToolError('Expected UUID must be 16 lowercase hexadecimal bytes')
    image = MachO(data)
    result = bytearray(data)
    pos = 28
    for _ in range(image.unpack('<I', 16)[0]):
        cmd, size = image.unpack('<II', pos)
        if cmd == 0x1b:
            result[pos + 8:pos + 24] = bytes.fromhex(expected)
            return bytes(result)
        pos += size
    raise ToolError('Linker did not emit the declared UUID command')


def command(profile, root, sdk=None):
    if profile.get('container'):
        image = profile['container']['image']
        if not re.fullmatch(r'(?:[^\s]+@)?sha256:[0-9a-f]{64}', image):
            raise ToolError('Linker container must have an immutable image digest')
        cmd = ['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none',
               '-v', str(Path(root).resolve()) + ':/work', '-w', '/work']
        if sdk:
            cmd += ['-v', str(Path(sdk).resolve()) + ':/sdk:ro']
        return cmd + [image, profile['container']['linker']]
    return profile['command']


def identity(profile, root, sdk=None):
    cmd = command(profile, root, sdk)
    run = subprocess.run(cmd + ['-v'], cwd=root, capture_output=True, text=True, timeout=30)
    if run.returncode:
        raise ToolError('Linker version probe failed: ' + run.stderr)
    version = (run.stdout + run.stderr).strip()
    if profile.get('container'):
        container = profile['container']
        info = subprocess.run(['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none',
                               container['image'], 'sha256sum', container['linker']], capture_output=True, text=True, check=True, timeout=30)
        binary_hash = info.stdout.split()[0]
    else:
        import shutil
        binary_hash = sha256(shutil.which(cmd[0]) or Path(root) / cmd[0])
    return {'family': profile['family'], 'version': version, 'binary_sha256': binary_hash,
            'invocation_sha256': digest(profile.get('container') or profile['command'])}


def fingerprint(profile, root, sdk=None):
    result = {'validated': False, 'reason': 'No linker profile configured', 'profile': None}
    if not profile:
        return result
    result['profile'] = {k: v for k, v in profile.items() if k not in ('validation', 'command')}
    try:
        info = identity(profile, root, sdk)
        result.update(info)
        if profile['family'] == 'synthetic-linker':
            result.update(validated=True, reason='Synthetic fixture linker; never a game linker')
        else:
            validation = load_json(Path(root) / profile['validation'])
            if (not validation.get('validated') or validation.get('identity') != info
                    or validation.get('profile_sha256') != digest(profile)
                    or validation.get('sdk') != inspect_sdk(sdk)
                    or len(validation.get('probes', [])) != 8):
                raise ToolError('Linker validation is absent, stale or incomplete')
            result.update(validated=True, reason=None)
    except (OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        result['reason'] = str(error)
    return result


def inspect_image(data, entry=None, imports=(), libraries=(), min_version=None, sdk_version=None, expected_bindings=None):
    """Inspect linked image commands/bindings independently of object relocation.

This deliberately does not reuse relocate() to validate the linker's output.
Dyld bind bytecode is bounds checked and resolved to named library ordinals.
"""
    image = MachO(data)
    if image.filetype != 2:
        raise ToolError('Linker output is not an ARMv7 executable')
    dylibs, segments, bind_regions = [], [], []
    thread_entry = None
    pos = 28
    count = struct.unpack_from('<I', data, 16)[0]
    versions = {}
    for _ in range(count):
        cmd, size = image.unpack('<II', pos)
        if cmd == 1:
            name, vmaddr, vmsize, fileoff, filesize, maxprot, prot, nsects, flags = image.unpack('<16s8I', pos + 8)
            image.check(fileoff, filesize)
            segments.append({'name': image.name(name), 'address': vmaddr, 'size': vmsize})
        elif cmd in (0xc, 0x80000018, 0x8000001f, 0x80000023):
            if size < 24:
                raise ToolError('Truncated dylib command')
            off = image.unpack('<I', pos + 8)[0]
            if off < 24 or off >= size:
                raise ToolError('Invalid dylib name offset')
            end = data.find(b'\0', pos + off, pos + size)
            if end < 0:
                raise ToolError('Unterminated dylib name')
            dylibs.append(image.name(data[pos + off:end]))
        elif cmd == 0x25:
            if size != 16:
                raise ToolError('Malformed iOS deployment command')
            versions['min'], versions['sdk'] = image.unpack('<II', pos + 8)
        elif cmd == 5:  # LC_UNIXTHREAD / ARM_THREAD_STATE
            if size != 84 or image.unpack('<II', pos + 8) != (1, 17):
                raise ToolError('Unsupported ARM thread entry command')
            thread_entry = image.unpack('<I', pos + 16 + 15 * 4)[0]
        elif cmd in (0x22, 0x80000022):
            if size != 48:
                raise ToolError('Malformed dyld info')
            fields = image.unpack('<10I', pos + 8)
            for name, i in (('bind', 2), ('weak', 4), ('lazy', 6)):
                off, length = fields[i:i + 2]
                image.check(off, length)
                bind_regions.append((name, off, length))
        pos += size
    if entry:
        choices = [s for s in image.symbols if s.defined and s.name == entry]
        if len(choices) != 1:
            raise ToolError('Linked entry symbol is absent or ambiguous: ' + entry)
        if thread_entry is not None and thread_entry != (choices[0].value | int(choices[0].thumb)):
            raise ToolError('ARM thread entry differs from declared entry symbol')
    bindings = []
    for kind, off, length in bind_regions:
        p, end = off, off + length
        ordinal, segment, address, name, addend = 0, None, 0, None, 0

        def leb(signed=False):
            nonlocal p
            value, shift = 0, 0
            while p < end and shift < 64:
                byte = data[p]; p += 1
                value |= (byte & 0x7f) << shift; shift += 7
                if byte < 128:
                    return value - (1 << shift) if signed and byte & 64 else value
            raise ToolError('Malformed bind LEB128')

        def emit():
            if not name or segment is None or segment >= len(segments) or address < 0 or address + 4 > segments[segment]['size']:
                raise ToolError('Bind target outside segment or missing symbol')
            if ordinal > len(dylibs) or ordinal < -3:
                raise ToolError('Invalid dylib ordinal')
            bindings.append({'kind': kind, 'symbol': name, 'ordinal': ordinal,
                             'library': dylibs[ordinal - 1] if ordinal > 0 else None,
                             'address': segments[segment]['address'] + address, 'addend': addend})

        while p < end:
            byte = data[p]; p += 1
            opcode, imm = byte & 0xf0, byte & 15
            if opcode == 0:
                if kind != 'lazy':
                    break
                ordinal, segment, address, name, addend = 0, None, 0, None, 0
            elif opcode == 0x10: ordinal = imm
            elif opcode == 0x20: ordinal = leb()
            elif opcode == 0x30: ordinal = imm - 16 if imm else 0
            elif opcode == 0x40:
                finish = data.find(b'\0', p, end)
                if finish < 0: raise ToolError('Unterminated bind symbol')
                name = image.name(data[p:finish]); p = finish + 1
            elif opcode == 0x50:
                if imm != 1: raise ToolError('Unsupported non-pointer binding')
            elif opcode == 0x60: addend = leb(True)
            elif opcode == 0x70: segment, address = imm, leb()
            # Dyld address arithmetic uses uintptr_t on this 32-bit image.
            # ld64 can encode a backwards delta as a 64-bit unsigned LEB.
            elif opcode == 0x80: address = (address + leb()) & 0xffffffff
            elif opcode == 0x90: emit(); address = (address + 4) & 0xffffffff
            elif opcode == 0xa0: emit(); address = (address + 4 + leb()) & 0xffffffff
            elif opcode == 0xb0: emit(); address = (address + 4 + 4 * imm) & 0xffffffff
            elif opcode == 0xc0:
                n, skip = leb(), leb()
                if n > 1000000: raise ToolError('Bind repetition resource limit')
                for _ in range(n): emit(); address = (address + 4 + skip) & 0xffffffff
            else: raise ToolError('Unsupported bind opcode')
    defined_imports = {b['symbol'] for b in bindings}
    if set(imports) - defined_imports:
        raise ToolError('Expected imports are not bound: ' + ', '.join(sorted(set(imports) - defined_imports)))
    if set(libraries) - set(dylibs):
        raise ToolError('Expected dylib load commands are absent')
    for symbol, library in (expected_bindings or {}).items():
        candidates = [b for b in bindings if b['symbol'] == symbol and b['kind'] != 'weak']
        if not candidates or any(b['library'] != library or b['addend'] != 0 for b in candidates):
            raise ToolError('Import binds to the wrong library or addend: ' + symbol)
    if min_version is not None and versions.get('min') != min_version:
        raise ToolError('iOS deployment version differs')
    if sdk_version is not None and versions.get('sdk') != sdk_version:
        raise ToolError('iOS SDK version differs')
    return {'architecture': 'armv7', 'libraries': dylibs, 'versions': versions, 'bindings': bindings,
            'sections': image.section_metadata(), 'entry': entry, 'thread_entry': thread_entry, 'runtime_validated': False}


def inputs(root, config):
    values = {'configuration': digest(config), 'original': config['provenance']['executable_sha256'],
              'sdk': inspect_sdk(config.get('sdk')), 'source_policy': dict(SOURCE_POLICY)}
    for unit in config['units']:
        path = Path(root) / ('build/units/' + unit['id'] + '.compile.json')
        values[unit['id']] = sha256(path) if path.exists() else None
        obj = Path(root) / ('build/units/' + unit['id'] + '.o')
        values[unit['id'] + '.o'] = sha256(obj) if obj.exists() else None
        for source in [unit['source'], *load_json(path).get('dependencies', {})] if path.exists() else [unit['source']]:
            source_path = local_path(root, source)
            values[source] = sha256(source_path) if source_path.exists() else None
    return values


def manifest_arguments(manifest, root):
    """Allow layout options, not arbitrary original objects/section payloads."""
    options = {'-dead_strip': 0, '-all_load': 0, '-no_dead_strip_inits_and_terms': 0,
               '-no_compact_unwind': 0, '-no_implicit_dylibs': 0, '-no_function_starts': 0,
               '-segaddr': 2, '-sectalign': 3, '-segalign': 1, '-seg1addr': 1,
               '-pagezero_size': 1, '-headerpad': 1, '-headerpad_max_install_names': 0,
               '-stack_size': 1, '-stack_addr': 1}
    flags, libraries = manifest.get('flags', []), manifest.get('libraries', [])
    if not isinstance(flags, list) or not isinstance(libraries, list) or not all(isinstance(x, str) for x in flags + libraries):
        raise ToolError('Link flags and libraries must be string arrays')
    i = 0
    while i < len(flags):
        option = flags[i]
        if option not in options or i + options[option] >= len(flags) or any(x.startswith('-') for x in flags[i + 1:i + 1 + options[option]]):
            raise ToolError('Unsupported link option; original objects/payloads are not allowed: ' + option)
        i += 1 + options[option]
    i = 0
    while i < len(libraries):
        if re.fullmatch(r'-l[A-Za-z0-9_.+]+', libraries[i]):
            i += 1
        elif libraries[i] == '-framework' and i + 1 < len(libraries) and re.fullmatch(r'[A-Za-z0-9_]+', libraries[i + 1]):
            i += 2
        else:
            raise ToolError('Libraries must be SDK -l names or -framework names')
    retained = manifest.get('retained_symbols', [])
    if (not isinstance(retained, list) or not all(isinstance(s, str) and
            re.fullmatch(r'_[A-Za-z_][A-Za-z_0-9.$]*', s) for s in retained)
            or len(set(retained)) != len(retained)):
        raise ToolError('Retained symbols must be distinct named linkage symbols')
    flags = list(flags)
    for name in retained:
        flags += ['-u', name]
    return flags, libraries


def run_link(root):
    from .build import configuration, comparisons
    root = Path(root)
    config = configuration(root)
    profile, manifest = config.get('linker'), config.get('link', {})
    info = fingerprint(profile, root, config.get('sdk'))
    out = root / 'build/link'
    out.mkdir(parents=True, exist_ok=True)
    image_path = out / 'Pirates'
    image_path.unlink(missing_ok=True)
    state = {'version': 1, 'state': 'unsupported', 'supported': info['validated'], 'profile': info,
             'complete_code': 0, 'complete_data': 0, 'complete_units': 0, 'runtime_validated': False,
             'scope': manifest.get('scope', 'replacement'), 'diagnostics': 'build/link/diagnostics.txt',
             'inputs': inputs(root, config), 'reason': info['reason'], 'participating_units': []}
    diagnostics = ''
    try:
        report = comparisons(root, include_link=False)
        if not manifest.get('enabled'):
            state.update(state='blocked', reason='Replacement linking is disabled until source coverage, object order and layout are reconstructed')
        elif not info['validated']:
            state.update(state='failed', reason='Configured linker is not validated: ' + str(info['reason']))
        elif config['provenance']['kind'] == 'game' and profile['family'] != 'ld64':
            raise ToolError('Synthetic linkers cannot link game candidates')
        elif state['scope'] == 'replacement' and (report['metrics']['matched_functions'] != report['metrics']['total_functions']
                or any(d['status'] != 'matched' for d in report['data'] if d['group_id'] != 'unowned-data')
                or len(config['units']) != len(report['units'])):
            state.update(state='blocked', reason='Replacement requires all original units, functions and owned data allocations to have verified source candidates; shared/anonymous data is checked in the final image')
        else:
            if any(u['error'] for u in report['units']):
                raise ToolError('Candidate objects are missing, stale or failed compilation')
            order = manifest.get('object_order', [])
            known = {u['id'] for u in config['units']}
            if len(set(order)) != len(order) or set(order) != known or not order:
                raise ToolError('Link manifest must order every configured source unit exactly once')
            if state['scope'] not in ('replacement', 'diagnostic'):
                raise ToolError('Link scope must be replacement or diagnostic')
            sdk_root = '/sdk' if profile.get('container') else config.get('sdk')
            flags, libraries = manifest_arguments(manifest, root)
            minimum, sdk_version = manifest.get('deployment_target', '4.2'), manifest.get('sdk_version', '5.1')
            args = ['-arch', 'armv7', '-ios_version_min', minimum, '-sdk_version', sdk_version, '-e', manifest['entry']]
            uuid = manifest.get('uuid')
            if not uuid:
                args += ['-no_uuid']
            if sdk_root:
                args += ['-syslibroot', sdk_root]
            args += flags + ['-o', 'build/link/Pirates'] + ['build/units/' + uid + '.o' for uid in order]
            for value in manifest.get('sdk_objects', []):
                if not config.get('sdk') or not isinstance(value, str) or not re.fullmatch(r'usr/lib/(?:g?crt1(?:\.3\.1)?)\.o', value):
                    raise ToolError('Only named startup objects from the verified SDK are supported')
                startup = Path(config['sdk']) / value
                if not startup.is_file() or not startup.resolve().is_relative_to(Path(config['sdk']).resolve()):
                    raise ToolError('SDK startup object is missing or outside the SDK')
                args += [sdk_root + '/' + value]
            args += libraries
            process = subprocess.run(command(profile, root, config.get('sdk')) + args, cwd=root, capture_output=True, text=True,
                                     env={**os.environ, 'LC_ALL': 'C', 'TZ': 'UTC', 'SOURCE_DATE_EPOCH': '0'}, timeout=300)
            diagnostics = process.stdout + process.stderr
            if process.returncode:
                raise ToolError('Linker failed with exit code ' + str(process.returncode))
            if uuid:
                original_uuid = MachO((root / config['provenance']['input']).read_bytes()).uuid
                if uuid != original_uuid:
                    raise ToolError('Declared UUID differs from the verified original identity')
                image_path.write_bytes(reproduce_uuid(image_path.read_bytes(), uuid))
                state['metadata_reproduction'] = {'uuid': uuid, 'scope': 'UUID identity field only; no address masking'}
            structure = inspect_image(image_path.read_bytes(), entry=manifest['entry'], imports=manifest.get('expected_imports', []),
                                      libraries=manifest.get('expected_libraries', []), min_version=version_number(minimum), sdk_version=version_number(sdk_version),
                                      expected_bindings=manifest.get('expected_bindings'))
            retained = manifest.get('retained_symbols', [])
            definitions = {s.name for s in MachO(image_path.read_bytes()).symbols if s.defined}
            if set(retained) - definitions:
                raise ToolError('Retained source symbols are absent from linked image: ' +
                                ', '.join(sorted(set(retained) - definitions)))
            if retained:
                structure['retained_symbols'] = retained
            state.update(state='linked', reason='Structurally linked; original image equality and runtime behavior are unverified',
                         image_sha256=sha256(image_path), structure=structure, participating_units=order)
            # No arbitrary reference hash, copied original objects, or partial
            # unit declarations can earn full-game completion.
            if state['scope'] == 'replacement' and state['image_sha256'] == config['provenance']['executable_sha256']:
                state.update(state='verified', reason='Source-only replacement has full coverage and byte-identical image',
                             complete_code=report['metrics']['matched_bytes'], complete_data=report['data_metrics']['total_bytes'],
                             complete_units=len(report['units']))
    except (OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        state.update(state='failed', reason=str(error))
        diagnostics += '\n' + str(error)
        image_path.unlink(missing_ok=True)
    (out / 'diagnostics.txt').write_text(diagnostics.replace(str(root.resolve()), '<workspace>'))
    write_json(out / 'status.json', state)
    print('Linking: ' + state['state'] + ' — ' + str(state['reason']))
    return state


def current_state(root, config):
    info = fingerprint(config.get('linker'), root, config.get('sdk'))
    fallback = {'state': 'unsupported' if not info['validated'] else 'blocked', 'supported': info['validated'],
                'profile': info, 'complete_code': 0, 'complete_data': 0, 'complete_units': 0,
                'reason': info['reason'] or 'Link step has not run', 'runtime_validated': False}
    try:
        state = load_json(Path(root) / 'build/link/status.json')
        if state['inputs'] != inputs(root, config) or state['profile'] != info:
            fallback.update(state='blocked', reason='Link inputs or linker changed; rebuild')
            return fallback
        if state['state'] in ('linked', 'verified'):
            if sha256(Path(root) / 'build/link/Pirates') != state['image_sha256']:
                raise ToolError('Linked image changed; rebuild')
        return {k: v for k, v in state.items() if k != 'inputs'}
    except (OSError, ValueError, KeyError) as error:
        return {**fallback, 'reason': str(error) if config.get('link', {}).get('enabled') else fallback['reason']}
