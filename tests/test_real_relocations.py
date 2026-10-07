"""Independent relocation fixtures emitted by modern Clang's assembler.

Hard-coded expected ARM/Thumb words ensure resolver tests don't simply repeat
its encoding functions. These fixtures are never game matching candidates.
"""
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.compare import compare_function
from tests.fixtures import reference, symbol


@unittest.skipUnless(shutil.which('clang'), 'Clang required for synthetic object fixture')
class RealRelocationTests(unittest.TestCase):
    def test_clang_movw_movt_pairs_against_fixed_original_words(self):
        for mode, expected in [('arm', '780605e3340241e31eff2fe1'), ('thumb', '45f27860c1f234207047')]:
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as tmp:
                path = Path(tmp)
                (path / 'probe.c').write_text('extern int target; int *probe(void) { return &target; }\n')
                subprocess.run(['clang', '-target', 'armv7-apple-ios4.2', '-O2', '-fno-pic', '-m' + mode,
                                '-c', 'probe.c', '-o', 'probe.o'], cwd=path, check=True, capture_output=True)
                candidate = MachO((path / 'probe.o').read_bytes())
                original = MachO(reference(bytes.fromhex(expected), mode, extras=[symbol('_target', 0x12345678, type=2)]))
                inv = recover(original)
                result = compare_function(original, inv, inv['functions'][0], candidate, details=True)
                self.assertEqual(result['status'], 'matched', result)
                self.assertEqual(len(result['relocations']), 2)
