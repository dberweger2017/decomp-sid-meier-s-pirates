"""Synthetic ABI forwarders: named targets, conservative selection, live calls."""
import copy
import json
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.abi_forwarders import validate, render
from tools.pirates.compare import compare_function
from tools.pirates.inventory import recover
from tools.pirates.macho import MachO
from tools.pirates.util import ToolError
from tests.fixtures import reference, symbol


def fixture():
    # push {r7,lr}; mov r7,sp; bl 0x1800; pop {r7,pc} at 0x1000.
    code = bytes.fromhex('80402de90d70a0e1fc0100eb8080bde8')
    original = MachO(reference(code, extras=[symbol('__ZN5ProbeD2Ev', 0x1800, type=2)]))
    inv = recover(original)
    f = inv['functions'][0]
    entry = {'id': f['id'], 'group_id': f['group_id'], 'symbol': f['symbol'],
             'original_name': 'Probe::Forward()', 'target_symbol': '__ZN5ProbeD2Ev',
             'target_name': 'Probe::~Probe() [base]', 'kind': 'method-forwarder',
             'return_type': 'void', 'parameters': ['void *'], 'target_parameters': ['void *']}
    return original, inv, {'version': 1, 'entries': [entry]}


class SelectionTests(unittest.TestCase):
    def test_conservative_named_target_and_deterministic_source(self):
        original, inv, spec = fixture()
        entries = validate(spec, inv, original)
        text = render(entries, entries[0]['group_id'])
        self.assertEqual(text, render(list(reversed(entries)), entries[0]['group_id']))
        self.assertIn('_target(a0);', text)
        self.assertIn('__asm__("__ZN5ProbeD2Ev")', text)
        self.assertNotIn('0x1800', text)
        self.assertNotIn('.byte', text)
        self.assertNotIn('asm volatile', text)

    def test_wrong_or_unproven_target_is_rejected(self):
        original, inv, spec = fixture()
        wrong = copy.deepcopy(spec)
        wrong['entries'][0]['target_symbol'] = '_unknown'
        with self.assertRaises(ToolError):
            validate(wrong, inv, original)
        other = MachO(reference(bytes.fromhex('80402de90d70a0e1fc0100eb8080bde8'),
                                extras=[symbol('__ZN5ProbeD2Ev', 0x1900, type=2)]))
        with self.assertRaises(ToolError):
            validate(spec, inv, other)

    def test_reject_adjustments_stack_arguments_duplicates_and_ambiguous_boundaries(self):
        original, inv, spec = fixture()
        variants = []
        for field, value in [('parameters', ['void *'] * 5), ('target_parameters', ['float']),
                             ('symbol', '_different'), ('target_symbol', '_bad"symbol'),
                             ('kind', 'complete-destructor')]:
            s = copy.deepcopy(spec)
            s['entries'][0][field] = value
            variants.append(s)
        duplicate = copy.deepcopy(spec)
        duplicate['entries'] *= 2
        variants.append(duplicate)
        for variant in variants:
            with self.subTest(variant=variant), self.assertRaises(ToolError):
                validate(variant, inv, original)
        inv['functions'][0]['ambiguities'] = ['overlapping range']
        with self.assertRaises(ToolError):
            validate(spec, inv, original)

    def test_import_forwarder_requires_independently_decoded_unique_stub(self):
        from tests.test_import_stubs import imported_image
        code = bytes.fromhex('80402de90d70a0e1fc0300eb8080bde8')
        original = MachO(imported_image(code=code))
        inv = recover(original)
        _, _, spec = fixture()
        e = spec['entries'][0]
        e.update(id=inv['functions'][0]['id'], group_id=inv['functions'][0]['group_id'],
                 symbol='_probe', target_symbol='_import', kind='function-forwarder')
        validate(spec, inv, original)
        for image in (imported_image(code=code, opcode=0xe1a00000),
                      imported_image(('_import', '_import'), code=code)):
            with self.assertRaises(ToolError):
                validate(spec, recover(MachO(image)), MachO(image))

    def test_registration_entries_have_local_source_and_no_implicit_registration(self):
        _, inv, spec = fixture()
        e = spec['entries'][0]
        e.update(symbol='__GLOBAL__I_probe', kind='registration-forwarder', parameters=[], target_parameters=[])
        text = render([e], e['group_id'])
        self.assertIn('static void pirates_registration_forwarder_', text)
        self.assertIn('__attribute__((used))', text)
        self.assertNotIn('__attribute__((constructor))', text)
        self.assertNotIn('new ', text)

    @unittest.skipUnless(shutil.which('clang++'), 'Modern Clang required for synthetic ABI fixture only')
    def test_compiled_wrapper_cannot_match_wrong_relocation_target(self):
        original, inv, spec = fixture()
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            for wrong in (False, True):
                s = copy.deepcopy(spec)
                if wrong:
                    s['entries'][0]['target_symbol'] = '_wrong'
                (root / 'probe.cpp').write_text(render(s['entries'], inv['functions'][0]['group_id']))
                subprocess.run(['clang++', '-target', 'armv7-apple-ios4.2', '-marm', '-O2',
                                '-fno-optimize-sibling-calls', '-c', 'probe.cpp', '-o', 'probe.o'],
                               cwd=root, capture_output=True, check=True)
                candidate = MachO((root / 'probe.o').read_bytes())
                result = compare_function(original, inv, inv['functions'][0], candidate, details=True)
                # Clang fixture frame/register choices may differ. Named target
                # resolution must still succeed only for the correct declaration.
                self.assertTrue(any(s.name == ('_wrong' if wrong else '__ZN5ProbeD2Ev') for s in candidate.symbols))
                self.assertEqual(result['relocations'][0]['status'], 'unresolved' if wrong else 'resolved')
                if wrong:
                    self.assertEqual(result['status'], 'unresolved')
                    self.assertFalse(result['byte_equal'])

    @unittest.skipUnless(shutil.which('clang++'), 'Host compiler required for synthetic call semantics only')
    def test_host_callee_receives_this_arguments_and_return_value(self):
        _, inv, spec = fixture()
        destructor = spec['entries'][0]
        boolean = dict(destructor, id='f-' + '1' * 20, symbol='_probe_boolean_entry',
                       target_symbol='_probe_boolean_target', return_type='bool',
                       parameters=['void *', 'const void *'], target_parameters=['void *', 'const void *'])
        pointer = dict(destructor, id='f-' + '2' * 20, symbol='_probe_pointer_entry',
                       target_symbol='_probe_pointer_target', return_type='void *',
                       parameters=['void *'], target_parameters=[])
        body = render([destructor, boolean, pointer], inv['functions'][0]['group_id'])
        body += '''
static int object, calls;
extern "C" void destruct_target(void *) __asm__("__ZN5ProbeD2Ev");
extern "C" void destruct_target(void *self) { if (self == &object) ++calls; }
extern "C" bool bool_target(void *, const void *) __asm__("_probe_boolean_target");
extern "C" bool bool_target(void *self, const void *other) { return self == other; }
extern "C" void *pointer_target(void) __asm__("_probe_pointer_target");
extern "C" void *pointer_target(void) { return &object; }
extern "C" void destruct_entry(void *) __asm__("_probe");
extern "C" bool bool_entry(void *, const void *) __asm__("_probe_boolean_entry");
extern "C" void *pointer_entry(void *) __asm__("_probe_pointer_entry");
int main() {
    destruct_entry(&object);
    if (calls != 1 || !bool_entry(&object, &object) || bool_entry(&object, 0)) return 1;
    if (pointer_entry(0) != &object) return 2;
    return 0;
}
'''
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            # Use the actual synthetic inventory name, independent of test fixtures.
            body = body.replace('__asm__("_probe");', '__asm__(' + json.dumps(destructor['symbol']) + ');')
            (root / 'probe.cpp').write_text(body)
            subprocess.run(['clang++', '-O2', 'probe.cpp', '-o', 'probe'], cwd=root, capture_output=True, check=True)
            subprocess.run([str(root / 'probe')], check=True, capture_output=True)
