#!/usr/bin/env python3
"""Identical-input/profile base/head builds and reviewable regression reports."""
import argparse
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from tools.pirates.configure import configure
from tools.pirates.compiler import profile_digest
from tools.pirates.report import regression, summary
from tools.pirates.util import load_json, write_json, ToolError, ninja_command

ROOT = Path(__file__).resolve().parents[1]


def publish_summary(output, markdown):
    (output / 'summary.md').write_text(markdown)
    if os.environ.get('GITHUB_STEP_SUMMARY'):
        with open(os.environ['GITHUB_STEP_SUMMARY'], 'a') as file:
            file.write(markdown)


def compare(base_path, head_path, output):
    output = Path(output)
    output.mkdir(parents=True, exist_ok=True)
    base, head = load_json(base_path), load_json(head_path)
    delta = regression(base, head)
    write_json(output / 'delta.json', delta)
    publish_summary(output, summary(base, head, delta))
    return delta


def stage(checkout, dest, tool_source, profile, validation=None):
    def ignored(directory, names):
        return [n for n in names if n in ('.git', '.venv', 'build', 'node_modules', 'research', 'artifacts')
                or n.endswith(('.ipa', '.sdk')) or n == '__pycache__']
    shutil.copytree(checkout, dest, ignore=ignored)
    tracked = subprocess.run(['git', '-C', str(checkout), 'ls-files', '-z'], capture_output=True, text=True)
    if tracked.returncode == 0:
        write_json(dest / 'build/site-source-allowlist.json', tracked.stdout.split('\0'))
    # Use one comparison engine for both revisions, preserving each revision's
    # sources and candidate manifest. The pre-tooling base starts at zero.
    shutil.rmtree(dest / 'tools', ignore_errors=True)
    shutil.copytree(tool_source / 'tools', dest / 'tools', ignore=shutil.ignore_patterns('__pycache__'))
    shutil.copy(tool_source / 'configure.py', dest / 'configure.py')
    (dest / 'config').mkdir(exist_ok=True)
    shutil.copy(tool_source / 'config/identity.json', dest / 'config/identity.json')
    shutil.copy(tool_source / 'config/inventory-lock.json', dest / 'config/inventory-lock.json')
    write_json(dest / 'config/ci-compiler.json', profile)
    if not (dest / 'config/candidates.json').exists():
        write_json(dest / 'config/candidates.json', {'version': 1, 'units': []})
    if validation and Path(validation).is_file():
        write_json(dest / profile['validation'], load_json(validation))
    # A staged CI workspace already carries the validated host/container profile.
    # Preserve it when staging a nested diagnostic link; the repository template
    # only names ld64 and is not an executable profile on the hosted runner.
    linker_path = next((tool_source / name for name in
                        ('build/linker/linker.json', 'config/ci-linker.json', 'config/linker.json')
                        if (tool_source / name).is_file()), None)
    if linker_path is not None:
        linker = load_json(linker_path)
        write_json(dest / 'config/ci-linker.json', linker)
        proof = tool_source / linker.get('validation', 'build/linker/validation.json')
        if proof.is_file():
            write_json(dest / linker['validation'], load_json(proof))


def build_one(root, ipa, sdk, output):
    output.mkdir(parents=True, exist_ok=True)
    try:
        configure(root, ipa=ipa, profile='config/ci-compiler.json', sdk=sdk,
                  linker='config/ci-linker.json' if (root / 'config/ci-linker.json').is_file() else None)
        process = subprocess.run(ninja_command(), cwd=root, capture_output=True, text=True)
        (output / 'build.log').write_text(process.stdout + process.stderr)
        code = process.returncode
        if code:
            # Missing source/headers can prevent Ninja reaching the report edge.
            # Refresh comparison evidence before exporting useful diagnostics.
            from tools.pirates.build import report
            try:
                report(root)
            except (ToolError, OSError, ValueError, KeyError):
                pass  # Configuration/input errors are already in build.log.
    except (ToolError, OSError, ValueError, KeyError) as e:
        (output / 'build.log').write_text(str(e) + '\n')
        code = 1
    for name in ('report.json', 'objdiff-report.json'):
        if (root / 'build' / name).exists():
            shutil.copy(root / 'build' / name, output / name)
    unit_dir = root / 'build/units'
    if unit_dir.exists():
        diagnostics = output / 'diagnostics'
        diagnostics.mkdir(exist_ok=True)
        for pattern in ('*.diagnostics.txt', '*.compile.json'):
            for path in unit_dir.glob(pattern):
                shutil.copy(path, diagnostics / path.name)
    for name in ('status.json', 'diagnostics.txt'):
        path = root / 'build/link' / name
        if path.is_file():
            (output / 'linking').mkdir(exist_ok=True)
            shutil.copy(path, output / 'linking' / name)
    return code


def compiler_profile_failure(base, profile, profile_path):
    baseline = Path(base) / profile.get('template_path', profile_path)
    candidates = Path(base) / 'config/candidates.json'
    if baseline.is_file() and candidates.is_file() and load_json(candidates).get('units'):
        if profile_digest(load_json(baseline)) != profile.get('template_sha256', profile_digest(profile)):
            return 'Shared compiler profile changed with existing candidates; establish a separate compiler baseline before comparing source progress'
    return None


def sdk_profile_failure(base, head):
    candidates = Path(base) / 'config/candidates.json'
    before, after = (Path(p) / 'config/sdk-lock.json' for p in (base, head))
    if candidates.is_file() and load_json(candidates).get('units') and before.is_file() and after.is_file():
        keys = ('manifest_sha256', 'version', 'build')
        if any(load_json(before).get(key) != load_json(after).get(key) for key in keys):
            return 'Shared SDK lock changed with existing candidates; establish a separate SDK baseline'
    return None


def linker_profile_failure(base, head):
    before = Path(base) / 'config/linker.json'
    after = Path(head) / 'config/linker.json'
    manifest = Path(base) / 'config/link.json'
    if before.is_file() and after.is_file() and manifest.is_file() and load_json(manifest).get('enabled'):
        from tools.pirates.linking import digest
        if digest(load_json(before)) != digest(load_json(after)):
            return 'Shared linker profile changed with linked candidates; establish a separate linker baseline'
    return None


def build(base, head, ipa, output, sdk=None, profile_path='config/compiler.json', validation=None):
    base, head, ipa, output = map(lambda p: Path(p).resolve(), (base, head, ipa, output))
    output.mkdir(parents=True, exist_ok=True)
    stage_root = head / 'build/ci-workspaces'
    if stage_root.exists():
        shutil.rmtree(stage_root)
    stage_root.mkdir(parents=True)
    profile = load_json(head / profile_path)
    profile_failure = compiler_profile_failure(base, profile, profile_path) or sdk_profile_failure(base, head) or linker_profile_failure(base, head)
    status = {}
    for name, checkout in (('base', base), ('head', head)):
        dest = stage_root / name
        stage(checkout, dest, head, profile, validation)
        status[name] = build_one(dest, ipa, sdk, output / name)
    write_json(output / 'build-status.json', status)
    if all((output / name / 'report.json').is_file() for name in ('base', 'head')):
        delta = compare(output / 'base/report.json', output / 'head/report.json', output)
        if profile_failure:
            delta['failures'].append(profile_failure)
        for name, code in status.items():
            if code:
                delta['failures'].append(name.title() + ' candidate build failed; see diagnostics/build.log')
        write_json(output / 'delta.json', delta)
        # Compare-only summaries and build status share the same generated file.
        markdown = summary(load_json(output / 'base/report.json'), load_json(output / 'head/report.json'), delta)
        (output / 'summary.md').write_text(markdown)
        if (any(status.values()) or profile_failure) and os.environ.get('GITHUB_STEP_SUMMARY'):
            with open(os.environ['GITHUB_STEP_SUMMARY'], 'a') as f:
                f.write('\n' + (profile_failure or 'Build failure: see uploaded diagnostics.') + '\n')
        return 1 if delta['failures'] else 0
    publish_summary(output, '## Pirates! build failed\n\nConfiguration or input verification failed. See uploaded base/head build.log. No original inputs are uploaded.\n')
    return 1


def demo(output):
    """Exercise the same CI checker with a deliberate synthetic regression."""
    from tests.test_workflow import fixture_workspace
    import tempfile
    output = Path(output).resolve()
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        fixture_workspace(root)
        subprocess.run(ninja_command(), cwd=root, check=True)
        output.mkdir(parents=True, exist_ok=True)
        shutil.copy(root / 'build/report.json', output / 'base.json')
        (root / 'src/value.h').write_text('#define VALUE 42\n')
        subprocess.run(ninja_command(), cwd=root, check=True)
        shutil.copy(root / 'build/report.json', output / 'head.json')
        delta = compare(output / 'base.json', output / 'head.json', output)
        print('Deliberate regression detected: ' + str(bool(delta['failures'])))
        return 1 if delta['failures'] else 0


def main():
    parser = argparse.ArgumentParser()
    commands = parser.add_subparsers(dest='command', required=True)
    cmp = commands.add_parser('compare')
    cmp.add_argument('--base', required=True, type=Path)
    cmp.add_argument('--head', required=True, type=Path)
    cmp.add_argument('--output', type=Path, default=Path('build/ci'))
    b = commands.add_parser('build')
    b.add_argument('--base', required=True, type=Path)
    b.add_argument('--head', type=Path, default=ROOT)
    b.add_argument('--ipa', required=True, type=Path)
    b.add_argument('--output', type=Path, default=Path('build/ci-artifacts'))
    b.add_argument('--sdk', type=Path)
    b.add_argument('--profile', default='config/compiler.json')
    b.add_argument('--validation', type=Path)
    d = commands.add_parser('demo-regression')
    d.add_argument('--output', type=Path, default=Path('build/ci-demo'))
    args = parser.parse_args()
    if args.command == 'compare':
        return 1 if compare(args.base, args.head, args.output)['failures'] else 0
    if args.command == 'demo-regression':
        return demo(args.output)
    return build(args.base, args.head, args.ipa, args.output, args.sdk, args.profile, args.validation)


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (ToolError, OSError, ValueError, KeyError) as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)
