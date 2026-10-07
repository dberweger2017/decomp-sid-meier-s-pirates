#!/usr/bin/env python3
import argparse
import importlib.metadata
import json
import subprocess
import sys
import webbrowser
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.build import comparisons, configuration
from tools.pirates.compiler import fingerprint
from tools.pirates.util import ToolError, load_json, ninja_command

ROOT = Path(__file__).resolve().parents[1]


def doctor(root):
    checks = []
    for name, version in [('capstone', '5.0.3'), ('ninja', '1.11.1.3')]:
        try:
            actual = importlib.metadata.version(name)
            checks.append({'name': name, 'ok': actual == version, 'detail': actual + ' (required ' + version + ')'})
        except importlib.metadata.PackageNotFoundError:
            checks.append({'name': name, 'ok': False, 'detail': 'Missing. Install requirements.txt in a virtual environment.'})
    try:
        binary = ninja_command()[0]
        checks.append({'name': 'ninja command', 'ok': True, 'detail': binary})
    except ToolError as e:
        checks.append({'name': 'ninja command', 'ok': False, 'detail': str(e)})
    try:
        config = configuration(root)
        checks.append({'name': 'original inputs', 'ok': True, 'detail': config['provenance']['executable_sha256']})
        inventory = load_json(root / 'build/inventory.json')
        coverage = inventory['coverage']
        checks.append({'name': 'inventory', 'ok': coverage['named_stabs_records'] == len(inventory['functions']),
                       'detail': f'{len(inventory["functions"])} functions / {len(inventory["groups"])} original groups'})
        info = fingerprint(config['compiler'], root, config.get('sdk'))
        synthetic = config['provenance']['kind'] == 'synthetic'
        checks.append({'name': 'historical compiler', 'ok': info['validated'] or synthetic,
                       'detail': info['version'] if info['validated'] else 'UNVALIDATED: ' + (info['reason'] or 'no evidence')})
        if config['compiler'].get('sdk_required'):
            sdk = Path(config.get('sdk') or 'missing-sdk')
            ok = sdk.is_dir() and (sdk / 'SDKSettings.plist').is_file()
            checks.append({'name': 'iOS SDK 5.1', 'ok': ok, 'detail': str(sdk) if ok else 'Supply locally with configure.py --sdk <iPhoneOS5.1.sdk>'})
    except (OSError, ValueError, KeyError) as e:
        checks.append({'name': 'configuration', 'ok': False, 'detail': str(e)})
    return checks


def main():
    parser = argparse.ArgumentParser(description='Pirates ARMv7 function workbench')
    sub = parser.add_subparsers(dest='command', required=True)
    diff = sub.add_parser('diff', help='Inspect byte verification and side-by-side assembly')
    diff.add_argument('function_id')
    diff.add_argument('--json', action='store_true')
    doc = sub.add_parser('doctor', help='Check inputs, compiler, SDK and pinned dependencies')
    doc.add_argument('--json', action='store_true')
    serve = sub.add_parser('serve', help='Open the browser and rebuild on source/header edits')
    serve.add_argument('--port', type=int, default=8765)
    serve.add_argument('--no-open', action='store_true')
    args = parser.parse_args()
    try:
        if args.command == 'doctor':
            checks = doctor(ROOT)
            if args.json:
                print(json.dumps(checks, indent=2))
            else:
                for c in checks:
                    print(('OK   ' if c['ok'] else 'FAIL ') + c['name'] + ': ' + c['detail'])
            return 0 if all(c['ok'] for c in checks) else 1
        if args.command == 'diff':
            result = comparisons(ROOT, details_id=args.function_id)
            if args.json:
                print(json.dumps(result, indent=2))
                return 0
            print(f'{result["id"]}  {result["symbol"]}\n{result["status"].upper()} · 0x{result["address"]:08x} · {result["mode"]} · {result["size"]} bytes')
            print('Verified byte equality: ' + str(result['byte_verified']) + '; assembly similarity: ' + str(result['similarity']))
            for reason in result['reasons']:
                print('Reason: ' + reason)
            if result.get('diagnostic_text'):
                print(result['diagnostic_text'])
            print(f'{"ORIGINAL":<65} CANDIDATE')
            for row in result['rows']:
                def format_row(value):
                    if not value:
                        return ''
                    return f'{value["address"]:08x}  {value["bytes"]:<10} {value["instruction"]:<10} {value["operands"]}'
                print(f'{format_row(row["original"]):<65} {format_row(row["candidate"])}' + ('  [' + ', '.join(row['differences']) + ']' if row['differences'] else ''))
            for rel in result['relocations']:
                print('Relocation: ' + json.dumps(rel, sort_keys=True))
            return 0
        if args.command == 'serve':
            configuration(ROOT)
            from tools.pirates.server import BrowserServer
            server = BrowserServer(ROOT, args.port)
            server.watcher.start()
            url = f'http://127.0.0.1:{server.server_port}'
            print('Pirates workbench: ' + url, flush=True)
            if not args.no_open:
                webbrowser.open(url)
            try:
                server.serve_forever()
            except KeyboardInterrupt:
                pass
            finally:
                server.server_close()
            return 0
    except (ToolError, OSError, KeyError, ValueError) as e:
        parser.exit(1, str(e) + '\n')


if __name__ == '__main__':
    sys.exit(main())
