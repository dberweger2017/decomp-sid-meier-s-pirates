import tempfile
import unittest
from pathlib import Path
from tools.ci import stage
from tools.pirates.util import load_json, write_json


class StagedLinkerTests(unittest.TestCase):
    def test_nested_workspace_preserves_validated_linker_profile(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory) / 'head'
            (root / 'tools').mkdir(parents=True)
            (root / 'configure.py').write_text('')
            (root / 'config').mkdir()
            for name in ('identity', 'inventory-lock'):
                write_json(root / ('config/' + name + '.json'), {})
            write_json(root / 'config/linker.json', {'command': ['ld64']})
            profile = {'command': ['/pinned/bin/ld'], 'validation': 'build/linker/validation.json'}
            write_json(root / 'config/ci-linker.json', profile)
            write_json(root / profile['validation'], {'validated': True})
            dest = Path(directory) / 'subset'
            stage(root, dest, root, {'family': 'synthetic'})
            self.assertEqual(load_json(dest / 'config/ci-linker.json'), profile)
            self.assertEqual(load_json(dest / profile['validation']), {'validated': True})
