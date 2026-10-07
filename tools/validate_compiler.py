#!/usr/bin/env python3
"""Validate supplied historical compiler/container using actual Mach-O probes.

This proves cross-build functionality and determinism, not exact equivalence to
an unknown Pirates compiler. Modern Clang is rejected.
"""
import hashlib
import json
import subprocess
import shutil
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.compiler import command
from tools.pirates.macho import MachO
from tools.pirates.util import load_json, write_json, ToolError, sha256

ROOT = Path(__file__).resolve().parents[1]


def validate(root, profile_path):
    root = Path(root)
    profile = load_json(root / profile_path)
    if profile['family'] != 'llvmgcc42':
        raise ToolError('Only the historical LLVM-GCC hypothesis may be validated')
    cmd = command(profile, root)
    version = subprocess.run(cmd + ['--version'], cwd=root, check=True, capture_output=True, text=True).stdout
    if not all(v in version for v in ('4.2.1', 'LLVM', '2336.9')) or 'clang' in version.lower():
        raise ToolError('Expected LLVM-GCC 4.2.1 / LLVM build 2336.9, not Clang or plain GCC')
    validation = {'validated': False, 'version': version.strip(), 'source_commit_hypothesis': 'c92700f7f0438a4bd5084145b9f351de63256e66'}
    invocation = profile.get('container') or profile['command']
    validation['command_sha256'] = hashlib.sha256(json.dumps(invocation, sort_keys=True, separators=(',', ':')).encode()).hexdigest()
    validation['compiler_sha256'] = profile['container']['image'].split('sha256:', 1)[1] if profile.get('container') else sha256(shutil.which(cmd[0]) or root / cmd[0])
    hashes = {}
    for language, extension in (('c', 'c'), ('c++', 'cpp')):
        for mode in ('arm', 'thumb'):
            pair = []
            for repeat in ('a', 'b'):
                folder = root / f'build/toolchain/probes/{extension}/{mode}/{repeat}'
                folder.mkdir(parents=True, exist_ok=True)
                (folder / 'value.h').write_text('#define VALUE 41\n')
                body = ('template<int N> int add(int x){return x+N;}\nextern "C" int probe(int x){return add<VALUE>(x);}'
                        if language == 'c++' else 'int probe(int x){return x+VALUE;}')
                source = 'probe.' + extension
                (folder / source).write_text('#include "value.h"\n' + body + '\n')
                relative = str(folder.relative_to(root))
                args = ['-x', language, '-march=armv7', '-mfloat-abi=softfp', '-O2', '-ffreestanding',
                        '-mthumb' if mode == 'thumb' else '-marm', '-MMD', '-MF', relative + '/probe.d']
                if language == 'c++':
                    args += ['-fno-exceptions', '-fno-rtti']
                args += ['-c', relative + '/' + source, '-o', relative + '/probe.o']
                run = subprocess.run(cmd + args, cwd=root, capture_output=True, text=True)
                (folder / 'diagnostics.txt').write_text(run.stdout + run.stderr)
                if run.returncode:
                    raise ToolError('Historical ' + mode + ' ' + language + ' Mach-O compilation failed; see ' + relative + '/diagnostics.txt')
                obj = MachO((folder / 'probe.o').read_bytes())
                if obj.filetype != 1 or not any(s.name == '_probe' and s.defined and s.thumb == (mode == 'thumb') for s in obj.symbols):
                    raise ToolError('Historical probe did not produce expected ARMv7 Mach-O function mode')
                if 'value.h' not in (folder / 'probe.d').read_text():
                    raise ToolError('Historical compiler did not emit included-header dependencies')
                pair.append(sha256(folder / 'probe.o'))
            if pair[0] != pair[1]:
                raise ToolError('Historical object bytes differ across build directories: ' + language + '/' + mode)
            hashes[extension + '/' + mode] = pair[0]
            validation[mode + '_probe'] = True
    validation.update(validated=True, c_probe=True, cxx_probe=True, reproducible_objects=True, probe_sha256=hashes)
    write_json(root / profile['validation'], validation)
    print('Validated historical cross-build and repeatable ARM/Thumb objects. Exact original compiler equivalence remains unproven.')


if __name__ == '__main__':
    try:
        validate(ROOT, sys.argv[1] if len(sys.argv) > 1 else 'config/compiler.json')
    except (ToolError, OSError, subprocess.CalledProcessError) as e:
        print('UNVALIDATED: ' + str(e), file=sys.stderr)
        sys.exit(1)
