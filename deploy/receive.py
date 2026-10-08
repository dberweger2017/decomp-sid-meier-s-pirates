#!/usr/bin/env python3
"""Restricted SSH deployment receiver; stdlib only, never runs uploaded code."""
import fcntl
import datetime
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import sys
import tarfile
import tempfile
import time
import urllib.request

REPOSITORY = 'dberweger2017/decomp-sid-meier-s-pirates'
INPUT_SHA = '0e1f1ef80636463821d6431ece18da6b269b69d7e9e11991b73f654812fd3c9e'
ROOT = Path('/opt/code/pirates')
REQUIRED_JOBS = {'Synthetic tooling (ubuntu-24.04)', 'Synthetic tooling (macos-15)',
                 'ARMv7 inventory and historical candidate progress'}
MAX_ARCHIVE = 128 * 1024 * 1024
MAX_EXPANDED = 768 * 1024 * 1024
STATIC = {'index.html', 'app.js', 'treemap.js', 'style.css', 'report.json', 'objdiff-report.json',
          'sources.json', 'link.json', 'manifest.json'}


def read_json(path):
    return json.loads(Path(path).read_text())


def atomic_json(path, value):
    tmp = path.with_name(path.name + '.next')
    tmp.write_text(json.dumps(value, sort_keys=True, indent=2) + '\n')
    tmp.chmod(0o644)
    os.replace(tmp, path)


def github(path):
    request = urllib.request.Request('https://api.github.com/repos/' + REPOSITORY + '/' + path,
                                     headers={'Accept': 'application/vnd.github+json', 'User-Agent': 'pirates-deploy'})
    with urllib.request.urlopen(request, timeout=20) as response:
        return json.load(response)


def verify_ci(commit, run_id, api=github):
    run = api('actions/runs/' + str(run_id))
    if (run.get('head_sha') != commit or run.get('head_branch') != 'main' or run.get('event') != 'push'
            or run.get('path') != '.github/workflows/build.yml'
            or run.get('head_repository', {}).get('full_name') != REPOSITORY
            or run.get('conclusion') not in (None, 'success')):
        raise ValueError('Only the main push build workflow may deploy')
    jobs = api(f'actions/runs/{run_id}/jobs?per_page=100')['jobs']
    passed = {j['name'] for j in jobs if j.get('status') == 'completed' and j.get('conclusion') == 'success'}
    if not REQUIRED_JOBS <= passed:
        raise ValueError('Required CI builds/tests have not succeeded')
    if api('git/ref/heads/main')['object']['sha'] != commit:
        raise ValueError('Superseded commit; current main must finish CI before deployment')


def allowed_file(name):
    return name in STATIC or re.fullmatch(r'functions/[fd]-[a-z0-9-]+\.json', name) is not None


def unpack(archive, dest):
    size, seen = 0, set()
    with tarfile.open(archive, 'r:gz') as tar:
        for member in tar:
            name = member.name
            if (not member.isfile() or not allowed_file(name) or name in seen
                    or str(PurePosixPath(name)) != name or member.size < 0):
                raise ValueError('Archive has a duplicate, unsafe, or unexpected member: ' + name)
            seen.add(name)
            size += member.size
            if size > MAX_EXPANDED or len(seen) > 40000:
                raise ValueError('Snapshot exceeds deployment limits')
            path = dest / name
            path.parent.mkdir(parents=True, exist_ok=True)
            with tar.extractfile(member) as source, path.open('wb') as target:
                shutil.copyfileobj(source, target)
            path.chmod(0o644)
    if not STATIC <= seen:
        raise ValueError('Snapshot lacks required files')
    manifest = read_json(dest / 'manifest.json')
    expected = manifest.get('files', {})
    if set(expected) != seen - {'manifest.json'}:
        raise ValueError('Manifest does not account for every exported file')
    for name, digest in expected.items():
        if hashlib.sha256((dest / name).read_bytes()).hexdigest() != digest:
            raise ValueError('Snapshot content hash mismatch: ' + name)
    return manifest


def validate_snapshot(dest, manifest, commit):
    report = read_json(dest / 'report.json')
    if manifest.get('version') != 1 or manifest.get('commit') != commit or manifest.get('kind') != 'game':
        raise ValueError('Snapshot must identify the requested real game commit')
    if not re.fullmatch(r'\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z', manifest.get('updated_at', '')):
        raise ValueError('Missing snapshot time')
    if (report.get('kind') != 'game' or report.get('input_sha256') != INPUT_SHA
            or not report.get('compiler', {}).get('validated')
            or len(report.get('functions', [])) != 9177 or len(report.get('units', [])) != 268):
        raise ValueError('Original identity, inventory or historical compiler validation failed')
    if manifest.get('metrics') != report['metrics']:
        raise ValueError('Manifest/report progress differs')
    if (report['metrics']['compile_errors'] or report['data_metrics']['compile_errors']
            or any(u.get('error') for u in report['units']) or report['linking']['state'] == 'failed'):
        raise ValueError('Failed build cannot replace the live website')
    ids = [r['id'] for r in report['functions'] + report['data']]
    expected = {'functions/' + rid + '.json' for rid in ids}
    if len(ids) != len(set(ids)) or expected != {p for p in manifest['files'] if p.startswith('functions/')}:
        raise ValueError('Detailed inventory coverage differs from report')
    return report


def check_regressions(previous, current):
    if previous['input_sha256'] != current['input_sha256']:
        raise ValueError('Deployment inputs changed')
    for key in ('functions', 'data', 'units'):
        old, new = {r['id']: r for r in previous[key]}, {r['id']: r for r in current[key]}
        if not old.keys() <= new.keys():
            raise ValueError('Deployment would remove inventory coverage')
        if key != 'units':
            for rid, record in old.items():
                if record['status'] == 'matched' and new[rid]['status'] != 'matched':
                    raise ValueError('Deployment would regress a verified match: ' + rid)
    if previous['linking']['state'] == 'verified' and current['linking']['state'] != 'verified':
        raise ValueError('Deployment would regress verified replacement linking')


def set_current(root, release):
    tmp = root / '.current.next'
    tmp.unlink(missing_ok=True)
    tmp.symlink_to('releases/' + release)
    os.replace(tmp, root / 'current')


def activate(root, candidate, manifest, run_id, health=None):
    """Called under a lock after provenance verification; rollback on failed health."""
    release = manifest['commit'] + '-' + hashlib.sha256((candidate / 'manifest.json').read_bytes()).hexdigest()[:12]
    state = root / 'state/deployment.json'
    previous = read_json(state) if state.exists() else None
    if previous and int(previous['run_id']) > int(run_id):
        raise ValueError('Older CI delivery cannot overwrite a newer deployment')
    if previous:
        check_regressions(read_json(root / 'releases' / previous['release'] / 'report.json'),
                          read_json(candidate / 'report.json'))
    deployed = root / 'releases' / release
    if not deployed.exists():
        candidate.rename(deployed)
    current = {'version': 1, 'release': release, 'commit': manifest['commit'],
               'updated_at': manifest['updated_at'], 'run_id': int(run_id), 'run_url': manifest.get('run_url'),
               'published_at': datetime.datetime.now(datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%SZ')}
    set_current(root, release)
    atomic_json(state, current)
    try:
        if health:
            health(current)
    except Exception:
        if previous:
            set_current(root, previous['release'])
            atomic_json(state, previous)
        else:
            (root / 'current').unlink(missing_ok=True)
            state.unlink(missing_ok=True)
        raise
    return current


def main():
    match = re.fullmatch(r'publish ([0-9a-f]{40}) ([1-9][0-9]{0,19})', os.environ.get('SSH_ORIGINAL_COMMAND', ''))
    if not match:
        raise ValueError('This key only accepts publish <commit> <CI run ID>')
    commit, run_id = match[1], int(match[2])
    with (ROOT / '.deploy.lock').open('a') as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        verify_ci(commit, run_id)
        with tempfile.TemporaryDirectory(prefix='.incoming-', dir=ROOT) as tmp:
            temp = Path(tmp)
            archive = temp / 'snapshot.tar.gz'
            with archive.open('wb') as target:
                copied = 0
                while chunk := sys.stdin.buffer.read(1024 * 1024):
                    copied += len(chunk)
                    if copied > MAX_ARCHIVE:
                        raise ValueError('Compressed snapshot exceeds limit')
                    target.write(chunk)
            candidate = temp / 'snapshot'
            candidate.mkdir(mode=0o755)
            manifest = unpack(archive, candidate)
            validate_snapshot(candidate, manifest, commit)
            # Recheck main immediately before promotion; downloads may take time.
            if github('git/ref/heads/main')['object']['sha'] != commit:
                raise ValueError('Main changed during upload; this release was superseded')
            current = activate(ROOT, candidate, manifest, run_id, serving_health)
            print('Published ' + current['release'])
            try:
                prune_releases(ROOT)
            except OSError as error:
                # Cleanup failure does not undo an already healthy deployment.
                print('Release cleanup deferred: ' + str(error), file=sys.stderr)


def serving_health(current):
    for path in ('deployment.json', 'releases/' + current['release'] + '/report.json'):
        with urllib.request.urlopen('http://127.0.0.1:8766/' + path, timeout=10) as response:
            value = json.load(response)
            if path == 'deployment.json' and value != current:
                raise ValueError('Serving health check returned a different deployment')
            if path.endswith('report.json') and value.get('input_sha256') != INPUT_SHA:
                raise ValueError('Serving health check returned a different input')


def prune_releases(root, keep=5, grace_seconds=86400):
    """Retain recent immutable data for existing browser requests and rollback."""
    state = read_json(root / 'state/deployment.json')
    releases = sorted((p for p in (root / 'releases').iterdir()
                       if p.is_dir() and not p.is_symlink()
                       and re.fullmatch(r'[0-9a-f]{40}-[0-9a-f]{12}', p.name)),
                      key=lambda p: p.stat().st_mtime, reverse=True)
    protected = {p.name for p in releases[:keep]} | {state['release']}
    for path in releases:
        if path.name not in protected and time.time() - path.stat().st_mtime > grace_seconds:
            shutil.rmtree(path)


if __name__ == '__main__':
    try:
        main()
    except Exception as error:
        print('Deployment rejected: ' + str(error), file=sys.stderr)
        sys.exit(1)
