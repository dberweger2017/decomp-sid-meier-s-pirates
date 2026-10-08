"""Portable read-only snapshot: no compiler, original executable or editor API."""
import copy
import hashlib
import json
import re
import shutil
import subprocess
import tempfile
from pathlib import Path
from .build import comparisons, configuration
from .report import objdiff_adapter
from .server import editable_files
from .util import ToolError, load_json, local_path, write_json

RECORD_ID = re.compile(r'[fd]-[a-z0-9-]+\Z')


def export_site(root, output, commit, updated_at, run_url=None):
    root, output = Path(root).resolve(), Path(output).resolve()
    if not re.fullmatch(r'[0-9a-f]{40}', commit):
        raise ToolError('Site commit must be a full lowercase Git SHA')
    if not re.fullmatch(r'\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}Z', updated_at):
        raise ToolError('Site update time must be UTC ISO 8601')
    if output.exists():
        raise ToolError('Export into a new directory to avoid mixed snapshots')
    if output == root or root.is_relative_to(output):
        raise ToolError('Export must not contain the source workspace')
    config = configuration(root)
    detailed = comparisons(root, details=True)
    saved = load_json(root / 'build/report.json')
    # Export fresh comparisons, but require the build/report edge to agree.
    # A save between build and export must never publish a stale verified match.
    for key in ('metrics', 'data_metrics', 'inventory_sha256', 'input_sha256', 'linking'):
        if saved.get(key) != detailed.get(key):
            raise ToolError('Report changed since build; run Ninja before exporting: ' + key)
    for key in ('functions', 'data'):
        state = lambda rows: [(r['id'], r['status'], r.get('similarity')) for r in rows]
        if state(saved[key]) != state(detailed[key]):
            raise ToolError('Comparison changed since build; run Ninja before exporting')

    def public(value):
        if isinstance(value, dict):
            # SDK file inventory and dependency paths belong to local evidence.
            return {k: public(v) for k, v in value.items() if k not in
                    ('file_records', 'dependencies', 'object', 'diagnostics', 'source_sha256', 'object_sha256')}
        if isinstance(value, list):
            return [public(v) for v in value]
        if isinstance(value, str):
            for path, replacement in ((str(root), '<workspace>'), (config.get('sdk'), '<sdk>')):
                if path:
                    value = value.replace(path, replacement)
        return value

    # Source is confined to configured local candidates/headers. In a checkout
    # only tracked files are exported, so an accidentally included private file
    # cannot be published. CI's staged workspace uses the checkout's allowlist.
    try:
        tracked = set(subprocess.check_output(['git', '-C', str(root), 'ls-files', '-z'],
                                              stderr=subprocess.DEVNULL).decode().split('\0'))
    except subprocess.CalledProcessError:
        tracked = None
    allowlist = root / 'build/site-source-allowlist.json'
    if allowlist.exists():
        tracked = set(load_json(allowlist))
    files, sources = {}, {}
    for unit in config['units']:
        paths = editable_files(root, unit['id'])
        paths = [p for p in paths if tracked is None or p in tracked]
        files[unit['id']] = paths
        for path in paths:
            sources[path] = public(local_path(root, path).read_text())
    diagnostics = {}
    for unit in detailed['units']:
        compiled = unit.get('compile')
        if compiled:
            diagnostics[unit['id']] = public(local_path(root, compiled['diagnostics']).read_text())
    link = public(copy.deepcopy(detailed['linking']))
    path = root / 'build/link/diagnostics.txt'
    link['diagnostic_text'] = public(path.read_text()) if path.is_file() else 'No link step has run.'
    output.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='.site-', dir=output.parent) as tmp:
        dest = Path(tmp) / 'snapshot'
        (dest / 'functions').mkdir(parents=True)
        report = public(copy.deepcopy(detailed))
        for key in ('functions', 'data'):
            for record in report[key]:
                if not RECORD_ID.fullmatch(record['id']):
                    raise ToolError('Invalid record ID')
                record['diagnostic_text'] = diagnostics.get(record.get('candidate_group_id', record['group_id']), '')
                write_json(dest / 'functions' / (record['id'] + '.json'), record)
            # Keep the tree/report small; details are fetched on selection.
            for record in report[key]:
                for field in ('rows', 'compile', 'compiler', 'diagnostic_text'):
                    record.pop(field, None)
        write_json(dest / 'report.json', report)
        write_json(dest / 'objdiff-report.json', objdiff_adapter(report))
        write_json(dest / 'sources.json', {'files': files, 'sources': sources})
        write_json(dest / 'link.json', link)
        web = Path(__file__).resolve().parents[1] / 'web'
        for name in ('app.js', 'style.css'):
            shutil.copy(web / name, dest / name)
        page = (web / 'index.html').read_text().replace("window.editToken = '__TOKEN__';", 'window.piratesHosted = true;')
        page = page.replace('Compare assembly, edit a candidate, and watch progress update.',
                            'Inspect assembly, candidate source, and verified CI progress.')
        (dest / 'index.html').write_text(page)
        hashes = {str(p.relative_to(dest)): hashlib.sha256(p.read_bytes()).hexdigest()
                  for p in sorted(dest.rglob('*')) if p.is_file()}
        manifest = {'version': 1, 'commit': commit, 'updated_at': updated_at,
                    'run_url': run_url, 'kind': report['kind'], 'metrics': report['metrics'], 'files': hashes}
        write_json(dest / 'manifest.json', manifest)
        dest.rename(output)
    return manifest
