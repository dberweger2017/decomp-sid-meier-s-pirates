#!/usr/bin/env python3
"""Import local SDK files or fetch the explicitly pinned public mirror."""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.sdk import inspect_sdk, require_sdk
from tools.pirates.util import ToolError, load_json, write_json

ROOT = Path(__file__).resolve().parents[1]


def fetch(root):
    root = Path(root)
    lock = load_json(root / 'config/sdk-lock.json')
    output = root / 'build/sdk/iPhoneOS5.1.sdk'
    if not output.exists():
        source = root / 'build/sdk/pinned-source'
        source.parent.mkdir(parents=True, exist_ok=True)
        if not source.exists():
            subprocess.run(['git', 'clone', '--filter=blob:none', '--no-checkout', lock['source'], str(source)], check=True)
        subprocess.run(['git', 'sparse-checkout', 'init', '--cone'], cwd=source, check=True)
        subprocess.run(['git', 'sparse-checkout', 'set', 'iPhoneOS5.1.sdk'], cwd=source, check=True)
        subprocess.run(['git', 'checkout', '--detach', lock['commit']], cwd=source, check=True)
        tree = subprocess.check_output(['git', 'rev-parse', 'HEAD:iPhoneOS5.1.sdk'], cwd=source, text=True).strip()
        if tree != lock['sdk_tree']:
            raise ToolError('Pinned SDK Git tree differs')
        require_sdk(source / 'iPhoneOS5.1.sdk', lock['manifest_sha256'])
        shutil.copytree(source / 'iPhoneOS5.1.sdk', output, symlinks=True)
    identity = require_sdk(output, lock['manifest_sha256'])
    write_json(root / 'build/sdk/provenance.json', {'identity': identity, 'source': lock})
    return output


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['fetch', 'import', 'check', 'stamp'])
    parser.add_argument('--sdk', type=Path)
    parser.add_argument('--pinned', action='store_true', help='Require the configured mirror content fingerprint')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/sdk-identity.json')
    args = parser.parse_args()
    if args.action == 'fetch':
        path = fetch(ROOT)
        print('Verified pinned SDK: ' + str(path))
        return
    if not args.sdk:
        raise ToolError('--sdk <directory> is required')
    if args.action == 'stamp':
        write_json(args.output, inspect_sdk(args.sdk))
        return  # Preserve diagnostics/report generation even for invalid inputs.
    expected = load_json(ROOT / 'config/sdk-lock.json')['manifest_sha256'] if args.pinned else None
    identity = require_sdk(args.sdk, expected)
    if args.action == 'import':
        write_json(ROOT / 'build/sdk/provenance.json', {'identity': identity, 'source': 'Locally supplied; no Apple authentication claimed'})
        from tools.pirates.configure import configure
        configure(ROOT, sdk=args.sdk)
    print(identity)


if __name__ == '__main__':
    try:
        main()
    except (ToolError, OSError, subprocess.CalledProcessError) as error:
        sys.exit(str(error))
