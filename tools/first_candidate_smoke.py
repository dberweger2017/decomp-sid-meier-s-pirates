#!/usr/bin/env python3
"""Verify real candidate/header rebuilds and a deliberate lost-match regression."""
import argparse
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.ci import stage
from tools.pirates.configure import configure
from tools.pirates.build import comparisons
from tools.pirates.report import regression
from tools.pirates.util import ToolError, load_json, write_json, ninja_command
ROOT = Path(__file__).resolve().parents[1]
FID = 'f-62cd60b098a1a0381e31'


def smoke(root=ROOT):
    root = Path(root)
    config = load_json(root / 'build/config.json')
    profile = config['compiler']
    output = root / 'build/candidate-smoke'
    output.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='candidate smoke ', dir=root / 'build') as temp:
        workspace = Path(temp) / 'workspace'
        stage(root, workspace, root, profile, root / profile['validation'])
        config['profile_path'] = 'config/ci-compiler.json'
        write_json(workspace / 'build/config.json', config)
        original = workspace / config['provenance']['input']
        original.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy(root / config['provenance']['input'], original)
        configure(workspace)
        subprocess.run(ninja_command(), cwd=workspace, check=True)
        base = comparisons(workspace)
        if not next(f for f in base['functions'] if f['id'] == FID)['byte_verified']:
            raise ToolError('First candidate is not verified before the regression test')
        source = workspace / 'src/powervr/PVRShellAPI.cpp'
        text = source.read_text()
        if text.count('return false;') != 1:
            raise ToolError('Unexpected first-candidate body; update the smoke test explicitly')
        source.write_text(text.replace('return false;', 'return true;'))
        subprocess.run(ninja_command(), cwd=workspace, check=True)
        head = comparisons(workspace)
        delta = regression(base, head)
        if FID not in delta['regressions'] or not delta['failures']:
            raise ToolError('Real verified-match regression was not detected')
        source.write_text(text)
        header = workspace / 'src/powervr/PVRShellAPI.h'
        header.write_text(header.read_text() + '\n// Included-header rebuild probe.\n')
        if comparisons(workspace, FID)['status'] != 'unresolved':
            raise ToolError('Stale candidate header was accepted before recompilation')
        subprocess.run(ninja_command(), cwd=workspace, check=True)
        if comparisons(workspace, FID)['status'] != 'matched':
            raise ToolError('First candidate did not recover after source/header rebuild')
        for name, value in (('base', base), ('head', head), ('delta', delta)):
            write_json(output / (name + '.json'), value)
        (output / 'summary.md').write_text('Verified source match → deliberate source regression → header rebuild/recovered match.\n\nRegressed function: ' + FID + '\n')
    print('Real candidate regression and included-header rebuild verified.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--workspace', type=Path, default=ROOT)
    args = parser.parse_args()
    try:
        smoke(args.workspace)
    except (ToolError, OSError, subprocess.SubprocessError) as error:
        sys.exit(str(error))
