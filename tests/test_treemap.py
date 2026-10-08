from pathlib import Path
import shutil
import subprocess
import unittest


class TreemapTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which('node'), 'Node is required for browser layout invariants')
    def test_layout_and_progress_semantics(self):
        root = Path(__file__).resolve().parents[1]
        result = subprocess.run(['node', 'tests/browser/treemap.test.cjs'], cwd=root,
                                capture_output=True, text=True, timeout=20)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
