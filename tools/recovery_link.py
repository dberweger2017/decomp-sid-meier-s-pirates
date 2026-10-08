#!/usr/bin/env python3
"""Link a selected recovered subset in an ignored, isolated Ninja workspace."""
import argparse
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.ci import stage
from tools.pirates.build import configuration
from tools.pirates.configure import configure
from tools.pirates.util import ToolError, load_json, write_json, ninja_command, local_path

ROOT = Path(__file__).resolve().parents[1]


def link_subset(root, descriptor):
    root = Path(root).resolve()
    specification = load_json(local_path(root, descriptor))
    selected = specification['candidate_groups']
    config = configuration(root)
    units = [u for u in load_json(root / config['candidates_path'])['units'] if u['group_id'] in selected]
    if not selected or len(set(selected)) != len(selected) or {u['group_id'] for u in units} != set(selected):
        raise ToolError('Diagnostic subset must name distinct configured original groups')
    manifest = specification['link']
    if manifest.get('scope') != 'diagnostic' or not manifest.get('enabled'):
        raise ToolError('Subset linking requires an enabled diagnostic manifest; no replacement credit')
    workspace = root / 'build/recovery-link/workspace'
    output = root / 'build/recovery-link'
    output.mkdir(parents=True, exist_ok=True)
    # A failed new attempt must not leave an earlier successful subset report.
    for name in ('status.json', 'diagnostics.txt', 'report.json', 'build.log'):
        (output / name).unlink(missing_ok=True)
    shutil.rmtree(workspace, ignore_errors=True)
    stage(root, workspace, root, config['compiler'], root / config['compiler']['validation'])
    original = workspace / config['provenance']['input']
    original.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(root / config['provenance']['input'], original)
    write_json(workspace / 'build/config.json', config)
    write_json(workspace / 'config/candidates.json', {'version': 1, 'units': units})
    write_json(workspace / 'config/link.json', manifest)
    configure(workspace, profile='config/ci-compiler.json', linker='config/ci-linker.json', link='config/link.json')
    with (output / 'build.log').open('w') as log:
        process = subprocess.run(ninja_command(), cwd=workspace, stdout=log, stderr=subprocess.STDOUT)
    for source, name in [('build/link/status.json', 'status.json'), ('build/link/diagnostics.txt', 'diagnostics.txt'),
                         ('build/report.json', 'report.json')]:
        path = workspace / source
        if path.exists():
            shutil.copyfile(path, output / name)
    if process.returncode:
        raise ToolError('Diagnostic subset build failed; see build/recovery-link/build.log and diagnostics.txt')
    state = load_json(output / 'status.json')
    if state['state'] != 'linked' or state['complete_units'] or state['complete_code'] or state['complete_data']:
        raise ToolError('Subset must structurally link while retaining zero replacement completion')
    print('Diagnostic subset linked: ' + state['image_sha256'] + '; zero replacement credit; runtime unverified')
    return state


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('descriptor', help='Repository-relative diagnostic subset descriptor')
    args = parser.parse_args()
    try:
        link_subset(ROOT, args.descriptor)
    except (ToolError, OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        sys.exit(str(error))
