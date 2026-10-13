import argparse
import hashlib
import json
from pathlib import Path
import shutil
import tempfile
import unittest

from tests.test_workflow import fixture_workspace
from tools.compiler_forensics import execute
from tools.pirates.util import ToolError, write_json


@unittest.skipUnless(shutil.which('clang') and shutil.which('ninja'), 'Clang and Ninja required')
class CompilerForensicsTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='compiler forensics ')
        self.root = Path(self.temp.name)
        self.fid = fixture_workspace(self.root)
        original = self.root / 'fixture.macho'
        write_json(self.root / 'config/identity.json', {'executable_sha256': hashlib.sha256(original.read_bytes()).hexdigest()})
        config = json.loads((self.root / 'build/config.json').read_text())
        self.gid = config['units'][0]['id']
        write_json(self.root / 'registry.json', {'compilers': {
            'control': {**config['compiler'], 'provenance': 'Synthetic test only'},
            'missing-historical': {'profile': 'missing.json', 'provenance': 'Historical placeholder', 'unavailable_reason': 'Supply genuine compiler'}
        }})
        write_json(self.root / 'sample.json', {'functions': [{
            'id': self.fid, 'group_id': self.gid, 'source': 'src/probe.c',
            'flags': [], 'source_confidence': 'Known synthetic source'
        }]})
        self.args = argparse.Namespace(action='batch', output=self.root / 'evidence',
            registry=self.root / 'registry.json', sample=self.root / 'sample.json',
            compilers='all', input=original, sdk=None, flag_search=False, cases=None)

    def tearDown(self):
        self.temp.cleanup()

    def test_source_comparison_preserves_missing_compilers_and_detects_header_changes(self):
        report = execute(self.args, self.root)
        self.assertFalse(report['compilers']['missing-historical']['available'])
        row = report['results'][0]
        self.assertTrue(row['comparison']['exact_relocated_bytes'])
        self.assertFalse(row['comparison']['progress_credit'])
        self.assertEqual(row['runtime_equivalence'], 'untested')
        self.assertIn('src/value.h', report['builds'][0]['dependencies'])
        self.assertTrue((self.args.output / 'summary.md').is_file())
        (self.root / 'src/value.h').write_text('#define VALUE 42\n')
        changed = execute(self.args, self.root)
        self.assertEqual(changed['results'][0]['comparison']['status'], 'different')
        self.assertNotEqual(report['builds'][0]['object_sha256'], changed['builds'][0]['object_sha256'])

    def test_preprocessed_include_payload_is_rejected_and_stale_object_deleted(self):
        good = execute(self.args, self.root)
        obj = self.root / good['builds'][0]['object']
        self.assertTrue(obj.exists())
        (self.root / 'src/value.h').write_text('#define VALUE 41\n#define PAYLOAD asm volatile("nop")\n')
        (self.root / 'src/probe.c').write_text('#include "value.h"\nint probe(int x) { PAYLOAD; return x + VALUE; }\n')
        bad = execute(self.args, self.root)
        self.assertEqual(bad['results'][0]['comparison']['status'], 'source_policy_rejected')
        self.assertFalse(obj.exists())
        self.assertFalse(bad['results'][0]['comparison']['exact_relocated_bytes'])

    def test_original_identity_and_object_group_cannot_be_changed_silently(self):
        sample = json.loads(self.args.sample.read_text())
        sample['functions'][0]['group_id'] = 'wrong-group'
        write_json(self.args.sample, sample)
        with self.assertRaisesRegex(ToolError, 'crosses original object groups'):
            execute(self.args, self.root)
        write_json(self.root / 'config/identity.json', {'executable_sha256': '0' * 64})
        with self.assertRaisesRegex(ToolError, 'does not match'):
            execute(self.args, self.root)
