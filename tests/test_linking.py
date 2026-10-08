"""Lifecycle tests use a fixture writer; SDK probes test the actual ld64."""
import copy
import inspect
import json
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from tools.pirates.configure import configure
from tools.pirates.build import comparisons
from tools.pirates.linking import inspect_image, current_state
from tools.pirates.util import load_json, write_json, ninja_command, ToolError
from tools.pirates.report import regression
from tests.fixtures import append_commands, macho, text, executable_symbols, symbol, reference
from tests.test_workflow import fixture_workspace


class ImageTests(unittest.TestCase):
    def test_versions_architecture_entry_and_expected_imports(self):
        original = reference(bytes.fromhex('1eff2fe1'))
        image = append_commands(original, [struct.pack('<4I', 0x25, 16, 0x40200, 0x50100)])
        parsed = inspect_image(image, entry='_probe', min_version=0x40200, sdk_version=0x50100)
        self.assertFalse(parsed['runtime_validated'])
        with self.assertRaisesRegex(ToolError, 'deployment'):
            inspect_image(image, min_version=0x40300)
        with self.assertRaisesRegex(ToolError, 'entry'):
            inspect_image(image, entry='_missing')
        with self.assertRaisesRegex(ToolError, 'not bound'):
            inspect_image(image, imports=['_wrong'])
        malformed = bytearray(image); struct.pack_into('<I', malformed, 28 + 4, 0xfffffff0)
        with self.assertRaises(ToolError): inspect_image(bytes(malformed))

    def test_link_failure_and_verified_image_regression_fail_ci(self):
        base = {'kind': 'synthetic', 'input_sha256': 'x', 'inventory_sha256': 'y', 'profile': {}, 'compiler': {},
                'coverage': {}, 'functions': [], 'units': [], 'linking': {'state': 'verified', 'profile': {}}}
        head = copy.deepcopy(base); head['linking']['state'] = 'failed'
        failures = regression(base, head)['failures']
        self.assertTrue(any('linking failed' in f for f in failures))
        self.assertTrue(any('replacement image regressed' in f for f in failures))


@unittest.skipUnless(shutil.which('clang'), 'Synthetic object tests require Clang')
class LinkLoopTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.fid = fixture_workspace(self.root)
        script = '''import struct,sys\nfrom pathlib import Path\nsys.path.insert(0,str(Path.cwd()))\nfrom tools.pirates.macho import MachO\n'''
        script += '\n'.join(inspect.getsource(f) for f in (symbol, executable_symbols, macho, text, reference, append_commands))
        script += '''\nif '-v' in sys.argv:\n print('synthetic image fixture writer; no real linker claim'); sys.exit(0)\nobj=MachO(Path(next(a for a in sys.argv if a.endswith('.o'))).read_bytes())\ns=next(s for s in obj.symbols if s.name=='_probe' and s.defined)\nsection=obj.section(s.section)\nraw=obj.bytes_at(s.value & ~1,section.size-((s.value & ~1)-section.address),s.section)\nimage=append_commands(reference(raw,'thumb'),[struct.pack('<4I',0x25,16,0x40200,0x50100)])\nPath(sys.argv[sys.argv.index('-o')+1]).write_bytes(image)\n'''
        (self.root / 'tools/fixture_linker.py').write_text(script)
        profile = {'family': 'synthetic-linker', 'command': [sys.executable, 'tools/fixture_linker.py']}
        write_json(self.root / 'config/linker.json', profile)
        gid = load_json(self.root / 'build/inventory.json')['groups'][0]['id']
        write_json(self.root / 'config/link.json', {'version': 1, 'enabled': True, 'scope': 'diagnostic',
                  'object_order': [gid], 'entry': '_probe', 'flags': [], 'libraries': []})
        configure(self.root)

    def tearDown(self):
        self.temp.cleanup()

    def build(self, ok=True):
        process = subprocess.run(ninja_command(), cwd=self.root, capture_output=True, text=True)
        self.assertEqual(process.returncode == 0, ok, process.stdout + process.stderr)
        return load_json(self.root / 'build/report.json'), process

    def test_diagnostic_link_header_edit_and_stale_image_rejection(self):
        base, _ = self.build()
        self.assertEqual(base['linking']['state'], 'linked')
        self.assertEqual(base['linking']['complete_units'], 0)
        before = base['linking']['image_sha256']
        unchanged, run = self.build()
        self.assertEqual(base, unchanged)
        self.assertNotIn('LINK + VALIDATE', run.stdout)
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        self.assertEqual(comparisons(self.root)['linking']['state'], 'blocked')
        after, run = self.build()
        self.assertIn('LINK + VALIDATE', run.stdout)
        self.assertNotEqual(before, after['linking']['image_sha256'])
        (self.root / 'build/link/Pirates').write_bytes(b'corrupt')
        self.assertEqual(comparisons(self.root)['linking']['state'], 'blocked')

    def test_link_failure_keeps_report_and_deletes_previous_image(self):
        base, _ = self.build()
        manifest = load_json(self.root / 'config/link.json')
        manifest['expected_imports'] = ['_missing_import']
        write_json(self.root / 'config/link.json', manifest)
        head, _ = self.build(ok=False)
        self.assertEqual(head['linking']['state'], 'failed')
        self.assertFalse((self.root / 'build/link/Pirates').exists())
        self.assertTrue((self.root / 'build/link/diagnostics.txt').exists())
        self.assertTrue(regression(base, head)['failures'])

    def test_partial_source_blocks_replacement_even_when_object_links(self):
        base, _ = self.build()
        manifest = load_json(self.root / 'config/link.json'); manifest['scope'] = 'replacement'
        write_json(self.root / 'config/link.json', manifest)
        # A changed header makes the only candidate differ from the original.
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        head, _ = self.build()
        self.assertEqual(head['linking']['state'], 'blocked')
        self.assertEqual(head['linking']['complete_units'], 0)

    def test_fully_covered_source_and_byte_identical_image_complete_unit(self):
        original = (self.root / 'fixture.macho').read_bytes()
        (self.root / 'fixture.macho').write_bytes(append_commands(original, [struct.pack('<4I', 0x25, 16, 0x40200, 0x50100)]))
        manifest = load_json(self.root / 'config/link.json'); manifest['scope'] = 'replacement'
        write_json(self.root / 'config/link.json', manifest)
        configure(self.root, fixture=self.root / 'fixture.macho', profile='config/fixture-compiler.json')
        report, _ = self.build()
        self.assertEqual(report['linking']['state'], 'verified')
        self.assertEqual(report['linking']['complete_units'], 1)
        self.assertEqual(report['linking']['complete_code'], report['metrics']['matched_bytes'])
        self.assertFalse(report['linking']['runtime_validated'])
