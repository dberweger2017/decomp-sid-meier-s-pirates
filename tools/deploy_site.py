#!/usr/bin/env python3
"""Package an allowlisted snapshot and send it to the restricted SSH receiver."""
import argparse
import gzip
import hashlib
import json
from pathlib import Path
import re
import subprocess
import tarfile
import tempfile
import sys
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from deploy.receive import allowed_file


def package(site, archive, commit):
    site, archive = Path(site), Path(archive)
    manifest = json.loads((site / 'manifest.json').read_text())
    if manifest['commit'] != commit:
        raise ValueError('Snapshot and deployment commit differ')
    names = sorted([*manifest['files'], 'manifest.json'])
    actual = {str(p.relative_to(site)) for p in site.rglob('*') if p.is_file()}
    if actual != set(names):
        raise ValueError('Snapshot contains missing or unexpected files')
    with archive.open('wb') as raw, gzip.GzipFile(fileobj=raw, mode='wb', mtime=0, filename='') as zipped:
        with tarfile.open(fileobj=zipped, mode='w', format=tarfile.USTAR_FORMAT) as tar:
            for name in names:
                path = site / name
                if not allowed_file(name) or path.is_symlink() or not path.resolve().is_relative_to(site.resolve()):
                    raise ValueError('Unsafe snapshot member: ' + name)
                if name in manifest['files'] and hashlib.sha256(path.read_bytes()).hexdigest() != manifest['files'][name]:
                    raise ValueError('Snapshot content changed: ' + name)
                info = tarfile.TarInfo(name)
                info.size, info.mode, info.mtime = path.stat().st_size, 0o644, 0
                with path.open('rb') as source:
                    tar.addfile(info, source)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--site', type=Path, required=True)
    parser.add_argument('--commit', required=True)
    parser.add_argument('--archive', type=Path)
    parser.add_argument('--run-id', type=int)
    parser.add_argument('--host')
    parser.add_argument('--user')
    parser.add_argument('--key', type=Path)
    parser.add_argument('--known-hosts', type=Path)
    args = parser.parse_args()
    if not re.fullmatch(r'[0-9a-f]{40}', args.commit):
        parser.error('Invalid commit')
    if args.archive:
        package(args.site, args.archive, args.commit)
        return
    if not all((args.run_id, args.host, args.user, args.key, args.known_hosts)):
        parser.error('Deployment requires run ID, host, user, key, and pinned known-hosts file')
    if not re.fullmatch(r'[a-zA-Z0-9.-]+', args.host) or not re.fullmatch(r'[a-zA-Z0-9_-]+', args.user):
        parser.error('Invalid SSH destination')
    with tempfile.TemporaryDirectory() as tmp:
        archive = Path(tmp) / 'snapshot.tar.gz'
        package(args.site, archive, args.commit)
        with archive.open('rb') as source:
            subprocess.run(['ssh', '-T', '-i', str(args.key), '-o', 'IdentitiesOnly=yes', '-o', 'BatchMode=yes',
                            '-o', 'StrictHostKeyChecking=yes', '-o', 'UserKnownHostsFile=' + str(args.known_hosts),
                            '-o', 'ConnectTimeout=20', args.user + '@' + args.host,
                            f'publish {args.commit} {args.run_id}'], stdin=source, check=True, timeout=600)


if __name__ == '__main__':
    main()
