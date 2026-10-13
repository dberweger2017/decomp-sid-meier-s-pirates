#!/usr/bin/env python3
"""Inventory an archived Xcode app without installing or selecting it globally.

Records desktop and iPhoneOS compilers separately, including support dylibs,
hashes, executable dependencies, build strings, manifests and runnable version
probes. Failures on modern hosts are evidence, not reason to modify the archive.
The SDK and Apple binaries remain local and must not be committed.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import plistlib
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
BUILD_STRINGS = re.compile(r'2336|5658|LLVM (?:build|version)|llvmgcc42|llvmCore|'
                           r'clang-[0-9]|arm[v0-9]*-apple-darwin|Configured with|'
                           r'cctools-[0-9]|PROJECT:(?:ld64|cctools)|libllvmgcc')


def digest(path):
    h = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            h.update(block)
    return h.hexdigest()


def capture(argv, env=None):
    try:
        run = subprocess.run([str(a) for a in argv], capture_output=True,
                             text=True, errors='replace', timeout=60, env=env)
        return {'argv': [str(a) for a in argv], 'returncode': run.returncode,
                'output': run.stdout + run.stderr}
    except (OSError, subprocess.TimeoutExpired) as error:
        return {'argv': [str(a) for a in argv], 'error': str(error)}


def inspect(app, output, run_tools=False, library_path=None):
    app = app.resolve()
    info_path = app / 'Contents/Info.plist'
    if not info_path.is_file():
        raise ValueError('Expected an Xcode .app with Contents/Info.plist')
    output.mkdir(parents=True, exist_ok=True)
    dev = app / 'Contents/Developer'
    result = {'version': 1, 'archive_path': str(app), 'apple_download_authenticated': False,
              'provenance_note': 'Supply distribution download/hash provenance separately; version strings alone do not authenticate the compiler.',
              'manifests': {}, 'tools': [], 'sdk_support': {},
              'execution_requested': run_tools}
    for p in (info_path, app / 'Contents/version.plist',
              dev / 'Platforms/iPhoneOS.platform/Info.plist',
              dev / 'Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk/SDKSettings.plist',
              dev / 'Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk/System/Library/CoreServices/SystemVersion.plist'):
        if p.is_file():
            result['manifests'][str(p.relative_to(app))] = {
                'sha256': digest(p), 'contents': plistlib.loads(p.read_bytes())}
    result['app_signature'] = capture(['/usr/bin/codesign', '-dv', '--verbose=4', app])
    result['app_signature_verification'] = capture(['/usr/bin/codesign', '--verify', '--deep', '--verbose=2', app])
    env = dict(os.environ)
    if library_path:
        # Explicit local runtime support; no global SDK or system-library edits.
        env['DYLD_LIBRARY_PATH'] = str(library_path.resolve())
        result['runtime_library_path'] = str(library_path.resolve())
    seen = set()
    directories = (dev / 'usr/bin', dev / 'usr/llvm-gcc-4.2',
                   dev / 'Platforms/iPhoneOS.platform/Developer/usr',
                   dev / 'Toolchains/XcodeDefault.xctoolchain/usr/bin',
                   dev / 'Toolchains/XcodeDefault.xctoolchain/usr/lib')
    for directory in directories:
        if not directory.exists():
            continue
        for p in sorted(directory.rglob('*')):
            if not p.is_file():
                continue
            name = p.name
            is_compiler = name.startswith('cc1') or 'llvm-gcc' in name or 'llvm-g++' in name
            if not (is_compiler or name in ('clang', 'clang++', 'as', 'ld', 'libllvmgcc.dylib',
                                            'libLTO.dylib', 'llc', 'llvm-config')):
                continue
            real = p.resolve()
            if real in seen:
                previous = next(entry for entry in result['tools'] if entry['real_path'] == str(real))
                previous.setdefault('aliases', []).append(str(p.relative_to(app)))
                continue
            seen.add(real)
            entry = {'path': str(p.relative_to(app)), 'real_path': str(real),
                     'scope': 'iphoneos-platform' if 'Platforms/iPhoneOS.platform/' in str(p) else 'shared-or-desktop',
                     'size': real.stat().st_size, 'sha256': digest(real),
                     'file': capture(['/usr/bin/file', real]),
                     'dependencies': capture(['/usr/bin/otool', '-L', real]),
                     'load_commands': capture(['/usr/bin/otool', '-l', real])}
            strings = capture(['/usr/bin/strings', real])
            entry['build_strings'] = [line for line in strings.get('output', '').splitlines()
                                      if BUILD_STRINGS.search(line)][:200]
            if run_tools and (is_compiler or name in ('clang', 'clang++')) and not name.startswith('cc1'):
                entry['version'] = capture([real, '-v'], env)
                entry['target'] = capture([real, '-dumpmachine'] if is_compiler else [real, '-###', '-arch', 'armv7', '-x', 'c', '-c', '/dev/null'], env)
                entry['specs'] = capture([real, '-dumpspecs'], env) if is_compiler else None
                entry['armv7_macros'] = capture([real, '-arch', 'armv7', '-dM', '-E', '-x', 'c', '/dev/null'], env)
                entry['target_help'] = capture([real, '--help', '-v', '-arch', 'armv7', '-x', 'c', '-c', '/dev/null', '-o', '/dev/null'], env)
                entry['armv7_driver'] = capture([real, '-###', '-arch', 'armv7', '-marm',
                    '-O2', '-miphoneos-version-min=4.2', '-x', 'c++', '-c', '/dev/null', '-o', '/dev/null'], env)
            elif run_tools and name in ('as', 'ld'):
                entry['version'] = capture([real, '-v'] + (['-o', '/dev/null'] if name == 'as' else []), env)
            result['tools'].append(entry)
            print(entry['path'], flush=True)
    sdk = dev / 'Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk'
    for sub in ('usr/include/c++', 'usr/lib', 'System/Library/Frameworks'):
        path = sdk / sub
        result['sdk_support'][sub] = sorted(p.name for p in path.iterdir()) if path.is_dir() else None
    # Package receipts and toolchain manifests can disambiguate build trains.
    for path in (app / 'Contents', dev / 'Toolchains', dev / 'usr/llvm-gcc-4.2'):
        if path.exists():
            result.setdefault('package_manifests', []).extend(
                str(p.relative_to(app)) for p in sorted(path.glob('*.plist')))
    # JSON handles plist bytes/dates while preserving their value representation.
    serialized = json.dumps(result, indent=2, sort_keys=True,
                            default=lambda value: value.hex() if isinstance(value, bytes) else str(value)) + '\n'
    (output / 'xcode-inventory.json').write_text(serialized)
    print('Inventory: ' + str(output / 'xcode-inventory.json'))
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('app', type=Path)
    parser.add_argument('--output', type=Path, default=ROOT / 'build/compiler-investigation/xcode-4.3.2')
    parser.add_argument('--run-tools', action='store_true')
    parser.add_argument('--library-path', type=Path)
    args = parser.parse_args()
    inspect(args.app, args.output, args.run_tools, args.library_path)
