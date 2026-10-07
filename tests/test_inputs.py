import hashlib
import plistlib
import tempfile
import unittest
import zipfile
from pathlib import Path
from tools.pirates.inputs import import_ipa
from tools.pirates.util import ToolError, sha256
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tests.fixtures import reference


class InputTests(unittest.TestCase):
    def ipa(self, root, data=None):
        data = data or reference(bytes.fromhex('1eff2fe1'))
        path = root / 'test.ipa'
        plist = {'CFBundleIdentifier': 'test.fixture', 'CFBundleVersion': '1', 'DTCompiler': 'fixture', 'DTXcode': 'fixture', 'DTSDKName': 'fixture'}
        with zipfile.ZipFile(path, 'w') as z:
            z.writestr('Payload/Test.app/Test', data)
            z.writestr('Payload/Test.app/Info.plist', plistlib.dumps(plist))
        identity = {'ipa_sha256': sha256(path), 'executable_sha256': hashlib.sha256(data).hexdigest(),
                    'executable_path': 'Payload/Test.app/Test', 'plist_path': 'Payload/Test.app/Info.plist',
                    'bundle_id': 'test.fixture', 'version': '1', 'expected_functions': 1, 'expected_groups': 1,
                    'observed_build': {'compiler': 'fixture', 'xcode': 'fixture', 'sdk': 'fixture'}}
        return path, identity

    def test_verified_import_accounts_for_every_record(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path, identity = self.ipa(root)
            provenance, inv = import_ipa(path, root, identity)
            self.assertEqual(inv['coverage']['named_stabs_records'], 1)
            self.assertEqual(sha256(root / provenance['input']), identity['executable_sha256'])

    def test_wrong_ipa_executable_metadata_and_coverage_are_rejected(self):
        for field, value in [('ipa_sha256', '0' * 64), ('executable_sha256', '0' * 64), ('bundle_id', 'wrong'), ('expected_functions', 2), ('expected_groups', 2)]:
            with self.subTest(field=field), tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp)
                path, identity = self.ipa(root)
                identity[field] = value
                with self.assertRaises(ToolError):
                    import_ipa(path, root, identity)
                self.assertFalse((root / 'build/inputs/Pirates').exists())

    def test_malformed_verified_archive(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path, identity = self.ipa(root)
            path.write_bytes(b'not a ZIP file')
            identity['ipa_sha256'] = sha256(path)
            with self.assertRaises(ToolError):
                import_ipa(path, root, identity)

    def test_current_game_identity_and_inventory_if_available(self):
        path = Path(__file__).resolve().parents[1] / 'research/Pirates-executable'
        if not path.exists():
            self.skipTest('Ignored original input is not present on this host')
        self.assertEqual(sha256(path), '0e1f1ef80636463821d6431ece18da6b269b69d7e9e11991b73f654812fd3c9e')
        inventory = recover(MachO(path.read_bytes()))
        self.assertEqual(inventory['coverage']['named_stabs_records'], 9177)
        self.assertEqual(inventory['coverage']['original_object_groups'], 268)
        self.assertEqual(len({f['id'] for f in inventory['functions']}), 9177)
        self.assertTrue(any(g['name'] == 'FireIncludeCpp.o' and len(g['source_paths']) > 1 for g in inventory['groups']))
