import json
import plistlib
import tempfile
import unittest
from pathlib import Path
from tools.pirates.sdk import inspect_sdk, require_sdk, header_flags
from tools.pirates.util import ToolError


def sdk_fixture(root):
    root = Path(root)
    root.mkdir(parents=True, exist_ok=True)
    (root / 'SDKSettings.plist').write_bytes(plistlib.dumps({'Version': '5.1', 'CanonicalName': 'iphoneos5.1'}))
    system = root / 'System/Library/CoreServices/SystemVersion.plist'
    system.parent.mkdir(parents=True)
    system.write_bytes(plistlib.dumps({'ProductVersion': '5.1', 'ProductBuildVersion': '9B176'}))
    header = root / 'usr/include/value.h'
    header.parent.mkdir(parents=True)
    header.write_text('#define SDK_VALUE 41\n')
    return root


class SDKTests(unittest.TestCase):
    def test_identity_is_path_independent_and_detects_content_changes(self):
        with tempfile.TemporaryDirectory() as temp:
            a, b = (sdk_fixture(Path(temp) / name) for name in ('a.sdk', 'b.sdk'))
            first = require_sdk(a)
            self.assertEqual(first, require_sdk(b))
            (a / 'usr/include/value.h').write_text('#define SDK_VALUE 42\n')
            self.assertNotEqual(first['sha256'], require_sdk(a)['sha256'])
            with self.assertRaisesRegex(ToolError, 'fingerprint differs'):
                require_sdk(a, first['sha256'])

    def test_wrong_build_and_escaping_links_are_rejected(self):
        with tempfile.TemporaryDirectory() as temp:
            root = sdk_fixture(Path(temp) / 'sdk')
            (root / 'outside').symlink_to('../secret')
            self.assertIn('escapes', inspect_sdk(root)['reason'])
            (root / 'outside').unlink()
            (root / 'System/Library/CoreServices/SystemVersion.plist').write_bytes(plistlib.dumps({'ProductVersion': '5.1', 'ProductBuildVersion': '9B206'}))
            self.assertIn('9B176', inspect_sdk(root)['reason'])

    def test_symlinks_are_preserved_in_identity_without_host_paths(self):
        with tempfile.TemporaryDirectory() as temp:
            root = sdk_fixture(Path(temp) / 'sdk')
            (root / 'alias').symlink_to('usr/include/value.h')
            (root / 'missing').symlink_to('absent')
            result = require_sdk(root)
            self.assertEqual(result['dangling_symlinks'], ['missing'])
            self.assertNotIn(temp, json.dumps(result))
            old = result['sha256']
            (root / 'alias').unlink()
            (root / 'alias').symlink_to('absent')
            self.assertNotEqual(old, require_sdk(root)['sha256'])

    def test_editor_and_container_share_header_layout_with_correct_roots(self):
        profile = {'family': 'llvmgcc42', 'container': {'image': 'unused'}}
        compiler = header_flags(profile, '/local/SDK', 'probe.mm')
        editor = header_flags(profile, '/local/SDK', 'probe.mm', editor=True)
        self.assertEqual([x.replace('/sdk', '/local/SDK') for x in compiler], editor)
        self.assertIn('/sdk/usr/include/c++/4.2.1', compiler)
        self.assertNotIn('/sdk/usr/include/c++/4.2.1', header_flags(profile, '/local/SDK', 'probe.m'))
