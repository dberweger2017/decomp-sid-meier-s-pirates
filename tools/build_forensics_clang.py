#!/usr/bin/env python3
"""Build the nearby published Apple Clang as a diagnostic compiler, never activate it.

Requires CMake, Ninja, a host C++14 compiler, and Python 3.9-3.12 (lib2to3).
The pinned archive includes LLVM. No SDK or proprietary Apple compiler is fetched.
"""
import argparse
import difflib
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import tarfile
import urllib.request
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.util import load_json, sha256, write_json, ToolError

ROOT = Path(__file__).resolve().parents[1]
HOST_FLAGS = '-fno-delete-null-pointer-checks -fno-strict-aliasing -std=c++14 -Wno-error -Wno-reserved-user-defined-literal -include cstdint -include cstddef'
REPAIRS = {
    'include/llvm/ADT/IntervalMap.h': [('this->map->newNode<', 'this->map->template newNode<')],
    'include/llvm/ADT/PointerUnion.h': [('Ty(Val).is<T>()', 'Ty(Val).template is<T>()'), ('Ty(Val).get<T>()', 'Ty(Val).template get<T>()')],
    'tools/clang/include/clang/AST/ASTVector.h': [('this->EndX < this->CapacityX', 'this->End < this->Capacity'), ('push_back(Elt);', 'push_back(Elt, C);')],
    'include/llvm/Support/CFG.h': [
        ('std::forward_iterator_tag,\n                                          Ptr, ptrdiff_t', 'std::forward_iterator_tag,\n                                          Ptr*, ptrdiff_t'),
        ('std::forward_iterator_tag, Ptr, ptrdiff_t', 'std::forward_iterator_tag, Ptr*, ptrdiff_t'),
        ('std::bidirectional_iterator_tag,\n                                          BB_, ptrdiff_t', 'std::bidirectional_iterator_tag,\n                                          BB_*, ptrdiff_t'),
        ('std::bidirectional_iterator_tag, BB_, ptrdiff_t', 'std::bidirectional_iterator_tag, BB_*, ptrdiff_t'),
        ('typedef typename super::pointer pointer;', 'typedef typename super::value_type pointer;')]
}


def prepare(source):
    changes = []
    for path, replacements in REPAIRS.items():
        p = source / path
        before = after = p.read_text()
        for old, new in replacements:
            if old not in after and new not in after:
                raise ToolError('Unexpected historical source for host repair: ' + path)
            after = after.replace(old, new)
        if before != after:
            p.write_text(after)
        changes.append({'path': path, 'patched_sha256': sha256(p), 'repairs': replacements})
    # This project is unrelated to building clang/llc and assumes an obsolete
    # host-header installation layout. Keep it outside project discovery.
    libcxx = source / 'projects/libcxx'
    if libcxx.exists():
        libcxx.rename(source.parent / 'libcxx-host-unused')
    if 'from .main import main' not in (source / 'utils/llvm-build/llvmbuild/__init__.py').read_text():
        subprocess.run([sys.executable, '-m', 'lib2to3', '-w', '-n', str(source / 'utils/llvm-build')], check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return changes


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--jobs', type=int, default=8)
    args = p.parse_args()
    locked = load_json(ROOT / 'toolchain/compiler-forensics-sources.lock.json')['apple_clang318']
    directory = ROOT / 'build/compiler-investigation'
    directory.mkdir(parents=True, exist_ok=True)
    archive = directory / 'clang-318.0.45.tar.gz'
    if not archive.exists():
        urllib.request.urlretrieve(locked['url'], archive)
    if sha256(archive) != locked['sha256']:
        raise ToolError('Historical Clang source archive hash mismatch')
    source = directory / ('clang-' + locked['commit']) / 'src'
    if not source.is_dir():
        with tarfile.open(archive) as tar:
            for entry in tar.getmembers():
                target = (directory / entry.name).resolve()
                if not target.is_relative_to(directory.resolve()) or entry.issym() or entry.islnk():
                    raise ToolError('Unsafe source archive member')
            tar.extractall(directory)
    changes = prepare(source)
    build = directory / 'clang318-build'
    configure = ['cmake', '-S', str(source), '-B', str(build), '-G', 'Ninja',
        '-DCMAKE_BUILD_TYPE=Release', '-DLLVM_TARGETS_TO_BUILD=ARM',
        '-DLLVM_INCLUDE_TESTS=OFF', '-DLLVM_INCLUDE_EXAMPLES=OFF', '-DLLVM_ENABLE_ASSERTIONS=OFF',
        '-DCMAKE_CXX_FLAGS=' + HOST_FLAGS, '-DCMAKE_POLICY_VERSION_MINIMUM=3.5',
        '-DPYTHON_EXECUTABLE=' + sys.executable]
    commands = [configure, ['ninja', '-C', str(build), '-j' + str(args.jobs), 'clang', 'llc', 'clang-headers']]
    provenance = {'source': locked, 'host_repairs': changes, 'commands': commands,
                  'python': sys.version, 'target_codegen_files_modified': False,
                  'caveat': 'Includes frontend ASTVector typo repairs and iterator-trait corrections. A locally ported source compiler is not the shipped Apple executable.'}
    write_json(directory / 'clang318-provenance.json', provenance)
    for index, cmd in enumerate(commands):
        with (directory / ('clang318-stage-' + str(index) + '.log')).open('w') as log:
            subprocess.run(cmd, cwd=ROOT, check=True, stdout=log, stderr=subprocess.STDOUT)
    compiler = build / 'bin/clang'
    profile = {'name': 'apple-clang318-source-port', 'family': 'clang-historical-diagnostic',
               'command': [str(compiler), '-ccc-host-triple', 'armv7-apple-darwin11', '-integrated-as'],
               'flags': ['-march=armv7-a', '-mfloat-abi=softfp', '-miphoneos-version-min=4.2'],
               'identity_files': [str(compiler), str(build / 'bin/llc')],
               'provenance': provenance}
    write_json(ROOT / 'build/compiler-forensics/clang318.json', profile)
    print('Diagnostic profile: build/compiler-forensics/clang318.json')


if __name__ == '__main__':
    main()
