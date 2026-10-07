#!/usr/bin/env python3
"""Reproduce the historical compiler experiment; never promote a failed build."""
import argparse
import hashlib
import json
import subprocess
import sys
import tarfile
import urllib.request
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.util import ToolError, load_json, write_json, sha256

ROOT = Path(__file__).resolve().parents[1]


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
                (['docker', 'run', '--rm', '--platform', 'linux/amd64', '-v', str(ROOT) + ':/work', 'pirates-compiler-feasibility',
                  '/bin/sh', 'toolchain/build-legacy.sh'], 'legacy-build')]
    for cmd, stage in commands:
        result['stage'] = stage
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
    print('Build finished; still UNVALIDATED until Mach-O/reproducibility probes pass.', file=sys.stderr)
    return 1


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['fetch', 'build', 'status'])
    args = parser.parse_args()
    if args.action == 'status':
        print(json.dumps(load_json(ROOT / 'toolchain/feasibility.json'), indent=2))
        return 1
    if args.action == 'fetch':
        fetch()
        return 0
    return build()


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (ToolError, OSError, ValueError) as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
