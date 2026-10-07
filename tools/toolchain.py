#!/usr/bin/env python3
"""Reproduce the historical compiler experiment; never promote a failed build."""
import argparse
import hashlib
import json
import subprocess
import shutil
import gzip
import sys
import tarfile
import urllib.request
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.util import ToolError, load_json, write_json, sha256

ROOT = Path(__file__).resolve().parents[1]


def export():
    cache = ROOT / 'build/toolchain'
    runtime = load_json(cache / 'runtime.json')
    if not runtime.get('validated'):
        raise ToolError('Refusing to export an unvalidated runtime')
    from tools.pirates.compiler import fingerprint
    info = fingerprint(load_json(cache / 'compiler.json'), ROOT)
    if not info['validated']:
        raise ToolError('Refusing to export an unvalidated runtime')
    archive = cache / 'runtime-image.tar'
    subprocess.run(['docker', 'image', 'save', '-o', str(archive), runtime['image']], check=True)
    compressed = archive.with_suffix('.tar.gz')
    with archive.open('rb') as source, compressed.open('wb') as output:
        with gzip.GzipFile(fileobj=output, mode='wb', mtime=0) as target:
            shutil.copyfileobj(source, target)
    archive.unlink()
    write_json(cache / 'runtime-export.json', {**runtime, 'archive': compressed.name, 'sha256': sha256(compressed)})
    return 0


def package():
    """Pin a local runtime only after validating its installed artifacts."""
    fetch()
    cache = ROOT / 'build/toolchain'
    write_json(cache / 'runtime.json', {'validated': False, 'reason': 'Runtime packaging and probes have not completed'})
    install = cache / 'legacy/install'
    for required in ('bin/llvm-g++', 'libexec/as/arm/as', 'arm-apple-darwin11/bin/as'):
        if not (install / required).is_file():
            raise ToolError('Historical installation is incomplete: ' + required)
    context = cache / 'runtime-context'
    shutil.rmtree(context, ignore_errors=True)
    shutil.copytree(install, context / 'install', symlinks=True)
    provenance = context / 'provenance'
    shutil.copytree(ROOT / 'toolchain', provenance)
    sources = provenance / 'sources'
    sources.mkdir()
    lock = load_json(ROOT / 'toolchain/sources.lock.json')
    for source in lock['sources']:
        shutil.copyfile(cache / (source['name'] + '.tar.gz'), sources / (source['name'] + '.tar.gz'))
    artifacts = {}
    for path in sorted(install.rglob('*')):
        key = str(path.relative_to(install))
        if path.is_symlink():
            artifacts[key] = {'link': str(path.readlink())}
        elif path.is_file():
            artifacts[key] = {'sha256': sha256(path)}
    write_json(provenance / 'installed-artifacts.json', artifacts)
    with (cache / 'runtime-build.log').open('w') as log:
        subprocess.run(['docker', 'build', '--platform', 'linux/amd64', '-f', str(ROOT / 'toolchain/Dockerfile.runtime'),
                        '-t', 'pirates-llvmgcc42', str(context)], cwd=ROOT, stdout=log, stderr=subprocess.STDOUT, check=True)
    image = subprocess.run(['docker', 'image', 'inspect', 'pirates-llvmgcc42', '--format', '{{.Id}}'],
                           check=True, capture_output=True, text=True).stdout.strip()
    profile = load_json(ROOT / 'config/compiler.json')
    from tools.pirates.compiler import profile_digest
    profile['template_sha256'] = profile_digest(profile)
    profile['template_path'] = 'config/compiler.json'
    profile['container'] = {'image': image, 'compiler': '/opt/pirates/bin/llvm-g++'}
    path = 'build/toolchain/compiler.json'
    write_json(ROOT / path, profile)
    from tools.validate_compiler import validate
    validate(ROOT, path)
    write_json(cache / 'runtime.json', {'image': image, 'platform': 'linux/amd64',
               'sources_lock_sha256': sha256(ROOT / 'toolchain/sources.lock.json'),
               'artifacts_manifest_sha256': sha256(provenance / 'installed-artifacts.json'), 'validated': True})
    print('Use: python configure.py --profile ' + path)
    return 0


def fetch():
    lock = load_json(ROOT / 'toolchain/sources.lock.json')
    cache = ROOT / 'build/toolchain'
    cache.mkdir(parents=True, exist_ok=True)
    for source in lock['sources']:
        archive = cache / (source['name'] + '.tar.gz')
        if not archive.exists() or sha256(archive) != source['sha256']:
            tmp = archive.with_suffix('.download')
            with urllib.request.urlopen(source['url'], timeout=60) as response, tmp.open('wb') as out:
                while True:
                    data = response.read(1024 * 1024)
                    if not data:
                        break
                    out.write(data)
            if sha256(tmp) != source['sha256']:
                tmp.unlink()
                raise ToolError('Pinned compiler source archive hash mismatch')
            tmp.replace(archive)
        dest = cache / (source['name'] + '-' + source['commit'])
        if not dest.is_dir():
            with tarfile.open(archive) as tar:
                # This pinned source release is trusted, but still reject archive
                # traversal and links escaping its extraction root.
                for member in tar.getmembers():
                    p = (cache / member.name).resolve()
                    if not p.is_relative_to(cache.resolve()) or member.issym() or member.islnk():
                        raise ToolError('Unsafe compiler source archive member: ' + member.name)
                tar.extractall(cache)
        print('Verified ' + source['tag'] + ' ' + source['sha256'])


def build():
    fetch()
    out = ROOT / 'build/toolchain'
    result = {'validated': False, 'sources_lock_sha256': sha256(ROOT / 'toolchain/sources.lock.json'), 'stage': 'environment', 'returncode': None}
    commands = [(['docker', 'build', '--platform', 'linux/amd64', '-f', 'toolchain/Dockerfile.feasibility', '-t', 'pirates-compiler-feasibility', '.'], 'environment'),
                (['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none', '-v', str(ROOT) + ':/work',
                  'pirates-compiler-feasibility', '/bin/sh', 'toolchain/build-assembler.sh'], 'assembler-build'),
                (['docker', 'run', '--rm', '--platform', 'linux/amd64', '--network', 'none', '-v', str(ROOT) + ':/work', 'pirates-compiler-feasibility',
                  '/bin/sh', 'toolchain/build-legacy.sh'], 'legacy-build')]
    for cmd, stage in commands:
        result['stage'] = stage
        result['returncode'] = None
        write_json(out / 'attempt.json', result)
        with (out / (stage + '.log')).open('w') as log:
            try:
                process = subprocess.run(cmd, cwd=ROOT, stdout=log, stderr=subprocess.STDOUT)
                result['returncode'] = process.returncode
            except OSError as e:
                log.write(str(e) + '\n')
                result['returncode'] = 1
        write_json(out / 'attempt.json', result)
        if result['returncode']:
            print(f'UNVALIDATED: {stage} failed. See build/toolchain/{stage}.log', file=sys.stderr)
            return 1
    result['stage'] = 'runtime-validation'
    result['returncode'] = None
    write_json(out / 'attempt.json', result)
    try:
        code = package()
    except (ToolError, OSError, subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        result.update(returncode=1, reason=str(e))
        write_json(out / 'attempt.json', result)
        raise
    result.update(validated=True, returncode=code)
    write_json(out / 'attempt.json', result)
    return code


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['fetch', 'build', 'package', 'export', 'status'])
    args = parser.parse_args()
    if args.action == 'status':
        result = load_json(ROOT / 'toolchain/feasibility.json')
        runtime = ROOT / 'build/toolchain/runtime.json'
        if runtime.exists():
            result['local_runtime'] = load_json(runtime)
            if result['local_runtime'].get('validated'):
                from tools.pirates.compiler import fingerprint
                result['local_runtime']['compiler'] = fingerprint(load_json(ROOT / 'build/toolchain/compiler.json'), ROOT)
        print(json.dumps(result, indent=2))
        return 0 if result.get('local_runtime', {}).get('compiler', {}).get('validated') else 1
    if args.action == 'fetch':
        fetch()
        return 0
    if args.action == 'package':
        return package()
    if args.action == 'export':
        return export()
    return build()


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (ToolError, OSError, ValueError, subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
