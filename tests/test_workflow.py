import copy
import json
import shutil
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.configure import configure
from tools.pirates.build import comparisons
from tools.pirates.report import regression, objdiff_adapter
from tools.pirates.util import write_json, load_json, ToolError
from tests.fixtures import reference

REPO = Path(__file__).resolve().parents[1]


def fixture_workspace(root, compiler='clang'):
    """Actual modern cross-compilation, explicitly synthetic."""
    root = Path(root)
    shutil.copytree(REPO / 'tools', root / 'tools', ignore=shutil.ignore_patterns('__pycache__'))
    shutil.copy(REPO / 'configure.py', root / 'configure.py')
    (root / 'config').mkdir(exist_ok=True)
    (root / 'src').mkdir(exist_ok=True)
    (root / 'src/value.h').write_text('#define VALUE 41\n')
    (root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE; }\n')
    command = [compiler, '-target', 'armv7-apple-ios4.2', '-O2', '-mthumb']
    subprocess.run(command + ['-c', 'src/probe.c', '-o', 'fixture.o'], cwd=root, check=True, capture_output=True)
    candidate = MachO((root / 'fixture.o').read_bytes())
    symbol = next(s for s in candidate.symbols if s.name == '_probe')
    sec = candidate.section(symbol.section)
    data = candidate.bytes_at(symbol.value & ~1, sec.size - ((symbol.value & ~1) - sec.address), symbol.section)
    (root / 'fixture.macho').write_bytes(reference(data, 'thumb'))
    inv = recover(MachO((root / 'fixture.macho').read_bytes()))
    profile = {'name': 'modern-clang-synthetic-only', 'family': 'clang-fixture', 'command': [compiler],
               'flags': command[1:], 'sdk_required': False}
    write_json(root / 'config/fixture-compiler.json', profile)
    manifest = {'version': 1, 'units': [{'group_id': inv['groups'][0]['id'], 'source': 'src/probe.c', 'flags': []}]}
    write_json(root / 'config/candidates.json', manifest)
    write_json(root / 'config/identity.json', {})  # unused with synthetic fixtures
    configure(root, fixture=root / 'fixture.macho', profile='config/fixture-compiler.json')
    return inv['functions'][0]['id']


@unittest.skipUnless(shutil.which('clang') and shutil.which('ninja'), 'Clang and Ninja are required for synthetic workflow tests')
class WorkflowTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='pirates fixtures ')
        self.root = Path(self.temp.name)
        self.fid = fixture_workspace(self.root)

    def tearDown(self):
        self.temp.cleanup()

    def build(self, ok=True):
        run = subprocess.run(['ninja'], cwd=self.root, capture_output=True, text=True)
        self.assertEqual(run.returncode == 0, ok, run.stdout + run.stderr)
        return load_json(self.root / 'build/report.json'), run

    def test_source_header_incremental_and_compile_failure_recovery(self):
        base, _ = self.build()
        self.assertEqual(base['functions'][0]['status'], 'matched', base)
        nochange, run = self.build()
        self.assertIn('no work to do', run.stdout)
        self.assertEqual(base, nochange)
        status = next((self.root / 'build/units').glob('*.compile.json'))
        before = status.stat().st_mtime_ns
        time.sleep(.02)
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        head, _ = self.build()
        self.assertGreater(status.stat().st_mtime_ns, before)
        self.assertEqual(head['functions'][0]['status'], 'different')
        delta = regression(base, head)
        self.assertIn(self.fid, delta['regressions'])
        self.assertTrue(delta['failures'])
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE + 1; }\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'different')
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { return x + VALUE; }\n')
        (self.root / 'src/value.h').write_text('#define VALUE this is invalid\n')
        failed, _ = self.build(ok=False)
        self.assertEqual(failed['functions'][0]['status'], 'compile_error')
        self.assertEqual(failed['metrics']['matched_functions'], 0)
        self.assertFalse(list((self.root / 'build/units').glob('*.o')))
        self.assertTrue(list((self.root / 'build/units').glob('*.diagnostics.txt')))
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')

    def test_reports_deterministic_across_workspace_paths(self):
        first, _ = self.build()
        with tempfile.TemporaryDirectory(prefix='pirates other host ') as tmp:
            fixture_workspace(Path(tmp))
            subprocess.run(['ninja'], cwd=tmp, check=True, capture_output=True)
            second = load_json(Path(tmp) / 'build/report.json')
            self.assertEqual(first, second)
            self.assertEqual((self.root / 'build/objdiff-report.json').read_bytes(), (Path(tmp) / 'build/objdiff-report.json').read_bytes())

    def test_header_or_flags_changed_cannot_verify_a_stale_object(self):
        self.assertEqual(self.build()[0]['functions'][0]['status'], 'matched')
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        result = comparisons(self.root, self.fid)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('dependency changed' in reason for reason in result['reasons']))
        (self.root / 'src/value.h').write_text('#define VALUE 41\n')
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['flags'] = ['-O0']
        write_json(self.root / 'config/candidates.json', manifest)
        configure(self.root)
        result = comparisons(self.root, self.fid)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('compile configuration changed' in reason for reason in result['reasons']))

    def test_adapter_has_proto_json_types_and_linking_stays_zero(self):
        report, _ = self.build()
        adapter = objdiff_adapter(report)
        self.assertIsInstance(adapter['measures']['totalCode'], str)
        self.assertEqual(adapter['measures']['matchedFunctions'], 1)
        self.assertEqual(adapter['measures']['completeCode'], '0')
        self.assertEqual(adapter['units'][0]['metadata']['complete'], False)
        self.assertEqual(adapter['units'][0]['functions'][0]['address'], '0')

    def test_inventory_loss_and_candidate_compile_failure_fail_regression_check(self):
        base, _ = self.build()
        head = copy.deepcopy(base)
        head['functions'] = []
        self.assertTrue(any('coverage disappeared' in f for f in regression(base, head)['failures']))
        head = copy.deepcopy(base)
        head['functions'][0]['status'] = 'compile_error'
        self.assertTrue(any('does not compile' in f for f in regression(base, head)['failures']))

    def test_clang_never_validates_historical_matching(self):
        profile = load_json(self.root / 'config/fixture-compiler.json')
        profile.update(family='llvmgcc42', validation='missing-validation.json')
        write_json(self.root / 'config/fixture-compiler.json', profile)
        configure(self.root)
        report, _ = self.build(ok=False)
        self.assertFalse(report['compiler']['validated'])
        self.assertEqual(report['metrics']['matched_functions'], 0)

    def test_missing_candidates_visible(self):
        write_json(self.root / 'config/candidates.json', {'version': 1, 'units': []})
        report, _ = self.build()
        self.assertEqual(report['metrics']['missing_candidates'], 1)
        self.assertEqual(report['metrics']['matched_functions'], 0)

    def test_corrupt_original_rejected(self):
        self.build()
        (self.root / 'build/inputs/Pirates').write_bytes(b'not the original')
        run = subprocess.run([sys.executable, 'tools/build.py', 'report'], cwd=self.root, capture_output=True, text=True)
        self.assertNotEqual(run.returncode, 0)
        self.assertIn('hash mismatch', run.stderr)

    def test_manifest_paths_cannot_escape_workspace(self):
        manifest = load_json(self.root / 'config/candidates.json')
        manifest['units'][0]['source'] = '../outside.cpp'
        write_json(self.root / 'config/candidates.json', manifest)
        with self.assertRaises(ToolError):
            configure(self.root)
