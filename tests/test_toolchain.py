import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
from tools import toolchain
from tools.pirates.util import ToolError, load_json, write_json


class ToolchainTests(unittest.TestCase):
    def test_incomplete_installation_cannot_keep_old_runtime_validation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            write_json(root / 'build/toolchain/runtime.json', {'validated': True})
            with patch.object(toolchain, 'ROOT', root), patch.object(toolchain, 'fetch'):
                with self.assertRaisesRegex(ToolError, 'Historical installation is incomplete'):
                    toolchain.package()
            self.assertFalse(load_json(root / 'build/toolchain/runtime.json')['validated'])
            with patch.object(toolchain, 'ROOT', root):
                with self.assertRaisesRegex(ToolError, 'Refusing to export an unvalidated runtime'):
                    toolchain.export()
