import json
from pathlib import Path
import os
import shutil
import subprocess
import tarfile
import tempfile
import unittest
from unittest.mock import patch
from tests.test_workflow import fixture_workspace
from tools.pirates.build import comparisons
from tools.pirates.configure import configure
from tools.pirates.macho import MachO
from tests.fixtures import reference
from tools.pirates.site import export_site
from tools.pirates.util import ToolError, load_json, write_json
from tools.deploy_site import package
from deploy.receive import unpack, activate, check_regressions, verify_ci, prune_releases, REQUIRED_JOBS, REPOSITORY

SHA = 'a' * 40
STAMP = '2026-10-08T00:00:00Z'


@unittest.skipUnless(shutil.which('clang') and shutil.which('ninja'), 'Clang and Ninja required')
class SiteTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name) / 'workspace'
        self.root.mkdir()
        self.fid = fixture_workspace(self.root)
        subprocess.run(['ninja'], cwd=self.root, check=True, capture_output=True)

    def export(self, name='site'):
        dest = Path(self.temp.name) / name
        export_site(self.root, dest, SHA, STAMP)
        return dest

    def test_export_agrees_with_terminal_and_contains_local_headers(self):
        dest = self.export()
        expected = comparisons(self.root, details_id=self.fid)
        detail = load_json(dest / 'functions' / (self.fid + '.json'))
        for key in ('rows', 'relocations', 'status', 'byte_verified', 'similarity'):
            self.assertEqual(expected[key], detail[key])
        sources = load_json(dest / 'sources.json')
        self.assertEqual({'src/probe.c', 'src/value.h'}, set(sources['sources']))
        self.assertEqual('synthetic', load_json(dest / 'manifest.json')['kind'])
        self.assertIn('window.piratesHosted = true;', (dest / 'index.html').read_text())
        self.assertNotIn('__TOKEN__', (dest / 'index.html').read_text())
        self.assertFalse(any(p.suffix in ('.o', '.ipa', '.macho') for p in dest.rglob('*')))

    def test_deterministic_files_and_archive(self):
        a, b = self.export('one'), self.export('two')
        for path in a.rglob('*'):
            if path.is_file():
                self.assertEqual(path.read_bytes(), (b / path.relative_to(a)).read_bytes())
        archives = [Path(self.temp.name) / x for x in ('a.tar.gz', 'b.tar.gz')]
        for site, archive in zip((a, b), archives):
            package(site, archive, SHA)
        self.assertEqual(archives[0].read_bytes(), archives[1].read_bytes())

    def test_changed_header_blocks_stale_verified_export(self):
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        with self.assertRaisesRegex(ToolError, 'Report changed'):
            self.export()

    def test_tracked_allowlist_excludes_untracked_headers(self):
        write_json(self.root / 'build/site-source-allowlist.json', ['src/probe.c'])
        sources = load_json(self.export() / 'sources.json')
        self.assertEqual(['src/probe.c'], list(sources['sources']))

    def test_export_round_trip_and_tampered_content_rejected(self):
        site = self.export()
        archive = Path(self.temp.name) / 'archive.tar.gz'
        package(site, archive, SHA)
        dest = Path(self.temp.name) / 'unpacked'
        dest.mkdir()
        self.assertEqual(SHA, unpack(archive, dest)['commit'])
        (site / 'report.json').write_text('{}')
        with self.assertRaisesRegex(ValueError, 'content changed'):
            package(site, archive, SHA)

    def test_atomic_promotion_rollback_and_old_delivery(self):
        root = Path(self.temp.name) / 'hosting'
        for part in ('state', 'releases'):
            (root / part).mkdir(parents=True)
        a = self.export('a')
        first = activate(root, a, load_json(a / 'manifest.json'), 10)
        b = self.export('b')
        # Different manifest makes a different immutable release.
        meta = load_json(b / 'manifest.json'); meta['updated_at'] = '2026-10-09T00:00:00Z'
        write_json(b / 'manifest.json', meta)
        with self.assertRaisesRegex(RuntimeError, 'health failed'):
            activate(root, b, meta, 11, lambda _: (_ for _ in ()).throw(RuntimeError('health failed')))
        self.assertEqual(first, load_json(root / 'state/deployment.json'))
        self.assertEqual((root / 'releases' / first['release']).resolve(), (root / 'current').resolve())
        c = self.export('c')
        with self.assertRaisesRegex(ValueError, 'Older CI'):
            activate(root, c, load_json(c / 'manifest.json'), 9)

    def test_live_regression_gate(self):
        old = load_json(self.export() / 'report.json')
        new = json.loads(json.dumps(old))
        new['functions'][0]['status'] = 'different'
        with self.assertRaisesRegex(ValueError, 'regress a verified match'):
            check_regressions(old, new)
        new['functions'] = []
        with self.assertRaisesRegex(ValueError, 'inventory coverage'):
            check_regressions(old, new)

    def test_bulk_details_agree_with_terminal_for_verified_unowned_data(self):
        function = load_json(self.root / 'build/inventory.json')['functions'][0]
        original = MachO((self.root / 'fixture.macho').read_bytes())
        raw = original.bytes_at(function['address'], function['size'], function['section'])
        (self.root / 'fixture.macho').write_bytes(reference(raw, 'thumb',
            extra_sections=[('__DATA', '__data', 0x2000, b'abcd', 0, [])]))
        configure(self.root, fixture=self.root / 'fixture.macho', profile='config/fixture-compiler.json')
        subprocess.run(['ninja'], cwd=self.root, check=True, capture_output=True)
        proof = {'state': 'verified', 'participating_units': [], 'complete_code': 0,
                 'complete_data': 4, 'complete_units': 0, 'reason': 'Synthetic linked image proof'}
        with patch('tools.pirates.linking.current_state', return_value=proof):
            bulk = comparisons(self.root, details=True)
            data = next(d for d in bulk['data'] if d['group_id'] == 'unowned-data')
            terminal = comparisons(self.root, details_id=data['id'])
            self.assertEqual(terminal['rows'], data['rows'])
            self.assertTrue(data['byte_verified'])
            self.assertEqual('61 62 63 64', data['rows'][0]['candidate'])


class DeploymentSecurityTests(unittest.TestCase):
    def test_retention_keeps_live_recent_and_last_five_without_following_symlinks(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'state').mkdir(); (root / 'releases').mkdir()
            paths = []
            for i in range(8):
                path = root / 'releases' / (f'{i:040x}-{i:012x}')
                path.mkdir(); os.utime(path, (i, i)); paths.append(path)
            write_json(root / 'state/deployment.json', {'release': paths[0].name})
            outside = root / 'keep-me'; outside.mkdir()
            link = root / 'releases' / ('f' * 40 + '-' + 'f' * 12); link.symlink_to(outside)
            with patch('deploy.receive.time.time', return_value=100000):
                prune_releases(root)
            self.assertTrue(paths[0].exists())
            self.assertFalse(paths[1].exists()); self.assertFalse(paths[2].exists())
            self.assertTrue(all(p.exists() for p in paths[3:]))
            self.assertTrue(link.is_symlink()); self.assertTrue(outside.exists())

    def test_archive_rejects_links_traversal_duplicates_and_binary_inputs(self):
        for name, kind in (('../app.js', tarfile.REGTYPE), ('app.js', tarfile.SYMTYPE),
                           ('original.ipa', tarfile.REGTYPE), ('functions/f-x.json', tarfile.LNKTYPE)):
            with self.subTest(name=name, kind=kind), tempfile.TemporaryDirectory() as tmp:
                archive = Path(tmp) / 'bad.tar.gz'
                with tarfile.open(archive, 'w:gz') as tar:
                    info = tarfile.TarInfo(name); info.type = kind
                    tar.addfile(info)
                with self.assertRaises(ValueError):
                    unpack(archive, Path(tmp))
        with tempfile.TemporaryDirectory() as tmp:
            archive = Path(tmp) / 'bad.tar.gz'
            with tarfile.open(archive, 'w:gz') as tar:
                tar.addfile(tarfile.TarInfo('app.js')); tar.addfile(tarfile.TarInfo('app.js'))
            with self.assertRaisesRegex(ValueError, 'duplicate'):
                unpack(archive, Path(tmp))

    def test_ci_provenance_rejects_prs_failed_jobs_and_superseded_commits(self):
        run = {'head_sha': SHA, 'head_branch': 'main', 'event': 'push', 'path': '.github/workflows/build.yml',
               'head_repository': {'full_name': REPOSITORY}, 'conclusion': None}
        jobs = {'jobs': [{'name': n, 'status': 'completed', 'conclusion': 'success'} for n in REQUIRED_JOBS]}
        ref = {'object': {'sha': SHA}}
        api = lambda p: ref if p.startswith('git/') else jobs if '/jobs?' in p else run
        verify_ci(SHA, 10, api)
        for key, value in (('event', 'pull_request'), ('head_branch', 'feature'), ('conclusion', 'failure')):
            before = run[key]; run[key] = value
            with self.assertRaisesRegex(ValueError, 'main push'):
                verify_ci(SHA, 10, api)
            run[key] = before
        jobs['jobs'][0]['conclusion'] = 'failure'
        with self.assertRaisesRegex(ValueError, 'have not succeeded'):
            verify_ci(SHA, 10, api)
        jobs['jobs'][0]['conclusion'] = 'success'; ref['object']['sha'] = 'b' * 40
        with self.assertRaisesRegex(ValueError, 'Superseded'):
            verify_ci(SHA, 10, api)
