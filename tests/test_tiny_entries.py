"""Partial leaf recovery: original linkage, instruction changes and live values."""
import copy
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from tests.fixtures import reference
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.compare import compare_function
from tools.pirates.util import ToolError
from tools.tiny_entries import validate, render


def leaf(code, kind='constant-body', ret='unsigned int', **extra):
    original = MachO(reference(bytes.fromhex(code), name=extra.pop('symbol', '_probe')))
    inv = recover(original)
    f = inv['functions'][0]
    e = {'id': f['id'], 'group_id': f['group_id'], 'symbol': f['symbol'],
         'original_name': 'Synthetic partial ABI leaf', 'kind': kind,
         'return_type': ret, 'parameters': ['void *'], 'internal': False, **extra}
    return original, inv, {'version': 1, 'entries': [e]}


class LeafTests(unittest.TestCase):
    def test_review_requires_correct_body_identity_and_original_linkage(self):
        original, inv, spec = leaf('0100a0e31eff2fe1', value=1)
        validate(spec, inv, original)
        for field, value in [('value', 0), ('value', True), ('return_type', 'float'),
                             ('internal', True), ('symbol', '_wrong'), ('parameters', ['invented type'])]:
            wrong = copy.deepcopy(spec)
            wrong['entries'][0][field] = value
            with self.subTest(field=field), self.assertRaises(ToolError):
                validate(wrong, inv, original)
        inv['functions'][0]['ambiguities'] = ['overlap']
        with self.assertRaises(ToolError):
            validate(spec, inv, original)

    @unittest.skipUnless(shutil.which('clang++'), 'Clang for synthetic ARM fixtures only')
    def test_verified_constant_and_adjusted_getter_reject_changed_source(self):
        for original, inv, spec in [leaf('0100a0e31eff2fe1', value=1),
                                   leaf('480090e51eff2fe1', 'adjusted-field-getter',
                                        offset=72, symbol='__ZThn180_N5Probe5ValueEv')]:
            validate(spec, inv, original)
            with tempfile.TemporaryDirectory() as tmp:
                root = Path(tmp)
                for wrong in (False, True):
                    text = render(spec['entries'], inv['functions'][0]['group_id'])
                    if wrong:
                        text = text.replace('return 1;', 'return 0;').replace('unknown[72]', 'unknown[76]')
                    (root / 'probe.cpp').write_text(text)
                    subprocess.run(['clang++', '-target', 'armv7-apple-ios4.2', '-marm',
                                    '-mfloat-abi=soft', '-O2', '-c', 'probe.cpp', '-o', 'probe.o'],
                                   cwd=root, check=True, capture_output=True)
                    result = compare_function(original, inv, inv['functions'][0],
                                              MachO((root / 'probe.o').read_bytes()))
                    self.assertEqual(result['status'], 'different' if wrong else 'matched')
                    self.assertEqual(result['byte_equal'], not wrong)

    @unittest.skipUnless(shutil.which('clang++'), 'Host compiler for synthetic semantics only')
    def test_local_duplicate_callbacks_link_and_leaf_values_execute(self):
        original, inv, spec = leaf('1eff2fe1', 'empty-body', 'void')
        base = spec['entries'][0]
        callback = dict(base, symbol='___tcf_1', kind='destruction-callback', internal=True)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for n in range(2):
                entry = dict(callback, id='f-' + str(n + 4) * 20)
                (root / (str(n) + '.cpp')).write_text(render([entry], base['group_id']))
                subprocess.run(['clang++', '-O2', '-c', str(n) + '.cpp', '-o', str(n) + '.o'],
                               cwd=root, capture_output=True, check=True)
            ident = dict(base, id='f-' + '1' * 20, symbol='_identity', kind='identity-body', return_type='void *')
            const = dict(base, id='f-' + '2' * 20, symbol='_constant', kind='constant-body', return_type='unsigned int', value=1)
            getter = dict(base, id='f-' + '3' * 20, symbol='_getter', kind='adjusted-field-getter', return_type='unsigned int', offset=72)
            text = render([ident, const, getter], base['group_id']) + '''
extern "C" void *identity(void *) __asm__("_identity");
extern "C" unsigned int constant(void *) __asm__("_constant");
extern "C" unsigned int getter(void *) __asm__("_getter");
struct View { unsigned char unknown[72]; unsigned int count; };
int main() { View v = {}; v.count = 39;
    return identity(&v) == &v && constant(&v) == 1 && getter(&v) == 39 ? 0 : 1; }
'''
            (root / 'main.cpp').write_text(text)
            subprocess.run(['clang++', '-O2', 'main.cpp', '0.o', '1.o', '-o', 'probe'],
                           cwd=root, capture_output=True, check=True)
            subprocess.run([str(root / 'probe')], check=True)

    @unittest.skipUnless(shutil.which('clang++'), 'Clang for synthetic Objective-C fixture only')
    def test_natural_objective_c_method_emission_matches(self):
        original = MachO(reference(bytes.fromhex('0100a0e31eff2fe1'), name='-[Probe orientation:]'))
        inv = recover(original)
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            (root / 'probe.mm').write_text('''
@interface Probe
- (signed char)orientation:(int)orientation;
@end
@implementation Probe
- (signed char)orientation:(int)orientation { return 1; }
@end
''')
            subprocess.run(['clang++', '-target', 'armv7-apple-ios4.2', '-marm', '-O2',
                            '-c', 'probe.mm', '-o', 'probe.o'], cwd=root, capture_output=True, check=True)
            candidate = MachO((root / 'probe.o').read_bytes())
            self.assertEqual(compare_function(original, inv, inv['functions'][0], candidate)['status'], 'matched')
