import struct
import unittest
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.compare import compare_function, assembly
from tools.pirates.relocations import encode_half, encode_thumb_branch
from tools.pirates.util import ToolError
from tests.fixtures import macho, text, symbol, reference, executable_symbols

ARM_BX = bytes.fromhex('1eff2fe1')
THUMB_BX = bytes.fromhex('7047')


class CoreTests(unittest.TestCase):
    def comparison(self, original, candidate, placements=None):
        orig, cand = MachO(original), MachO(candidate)
        inv = recover(orig)
        return compare_function(orig, inv, inv['functions'][0], cand, placements=placements, details=True)

    def test_arm_and_thumb_exact_and_changed_instruction(self):
        for mode, data, changed in [('arm', bytes.fromhex('010080e2') + ARM_BX, bytes.fromhex('020080e2') + ARM_BX),
                                    ('thumb', bytes.fromhex('0130') + THUMB_BX, bytes.fromhex('0230') + THUMB_BX)]:
            syms = [symbol('_probe', desc=8 if mode == 'thumb' else 0)]
            self.assertEqual(self.comparison(reference(data, mode), macho([text(data)], syms))['status'], 'matched')
            r = self.comparison(reference(data, mode), macho([text(changed)], syms))
            self.assertEqual(r['status'], 'different')
            self.assertFalse(r['byte_equal'])
            self.assertTrue(any('operands' in row['differences'] for row in r['rows']))

    def test_register_differences(self):
        a = bytes.fromhex('000080e2') + ARM_BX
        b = bytes.fromhex('001080e2') + ARM_BX
        r = self.comparison(reference(a), macho([text(b)], [symbol('_probe')]))
        self.assertTrue(any('registers' in row['differences'] for row in r['rows']))

    def test_literal_pool_and_vanilla_target(self):
        data = bytes.fromhex('00009fe5') + ARM_BX + struct.pack('<I', 0x2001)
        original = reference(data, extras=[symbol('_data', 0x2001, section=2)],
                             extra_sections=[('__DATA', '__data', 0x2001, b'1234', 0, [])])
        # VANILLA pointer to odd-address data must not clear the low bit.
        rel = {'type': 0, 'length': 2, 'address': 8, 'external': True, 'symbol': 1}
        cand = macho([text(data[:8] + b'\0' * 4, relocations=[rel])], [symbol('_probe'), symbol('_data', section=0, type=1)])
        r = self.comparison(original, cand)
        self.assertEqual(r['status'], 'matched')
        self.assertEqual(r['relocations'][0]['target_address'], 0x2001)
        self.assertEqual(r['rows'][-1]['original']['kind'], 'literal')

    def test_wrong_relocation_target_cannot_match(self):
        original = reference(struct.pack('<I', 0x2000), extras=[symbol('_right', 0x2000, type=2), symbol('_wrong', 0x3000, type=2)])
        rel = {'type': 0, 'length': 2, 'address': 0, 'external': True, 'symbol': 1}
        candidate = macho([text(b'\0' * 4, relocations=[rel])], [symbol('_probe'), symbol('_wrong', section=0, type=1)])
        r = self.comparison(original, candidate)
        self.assertEqual(r['status'], 'different')
        self.assertEqual(r['relocations'][0]['target'], '_wrong')

    def test_unsupported_and_unresolved_are_never_matches(self):
        for t, target in [(4, '_probe'), (0, '_missing')]:
            rel = {'type': t, 'length': 2, 'address': 0, 'external': True, 'symbol': 1}
            same = b'\0' * 4
            r = self.comparison(reference(same), macho([text(same, relocations=[rel])],
                               [symbol('_probe'), symbol(target, section=0, type=1)]))
            self.assertEqual(r['status'], 'unresolved')
            self.assertTrue(r['byte_equal'])

    def test_arm_branch(self):
        target = 0x2000
        data = struct.pack('<I', 0xeb000000 | ((target - 0x1000 - 8) >> 2)) + ARM_BX
        rel = {'type': 5, 'length': 2, 'pcrel': True, 'address': 0, 'external': True, 'symbol': 1}
        r = self.comparison(reference(data, extras=[symbol('_target', target, type=2)]),
                            macho([text(struct.pack('<I', 0xebfffffe) + ARM_BX, relocations=[rel])],
                                  [symbol('_probe'), symbol('_target', section=0, type=1)]))
        self.assertEqual(r['status'], 'matched')
        self.assertEqual(r['relocations'][0]['target_address'], target)

    def test_thumb2_branch(self):
        base_word = 0xf800f000  # Thumb BL, first halfword low in little-endian word
        original = struct.pack('<I', encode_thumb_branch(base_word, 0x2000 - 0x1000 - 4)) + THUMB_BX
        candidate = struct.pack('<I', encode_thumb_branch(base_word, -4)) + THUMB_BX
        rel = {'type': 6, 'length': 2, 'pcrel': True, 'address': 0, 'external': True, 'symbol': 1}
        r = self.comparison(reference(original, 'thumb', extras=[symbol('_target', 0x2000, type=2, desc=8)]),
                            macho([text(candidate, relocations=[rel])], [symbol('_probe', desc=8), symbol('_target', section=0, type=1)]))
        self.assertEqual(r['status'], 'matched')

    def test_branch_interworking_is_unresolved_even_if_bytes_agree(self):
        data = struct.pack('<I', 0xeb000000 | ((0x2000 - 0x1000 - 8) >> 2)) + ARM_BX
        original = reference(data, extras=[symbol('_thumb', 0x2000, type=2, desc=8)])
        reloc = {'type': 5, 'length': 2, 'pcrel': True, 'address': 0, 'external': True, 'symbol': 1}
        candidate = macho([text(struct.pack('<I', 0xebfffffe) + ARM_BX, relocations=[reloc])],
                          [symbol('_probe'), symbol('_thumb', section=0, type=1)])
        result = self.comparison(original, candidate)
        self.assertEqual(result['status'], 'unresolved')
        self.assertTrue(any('interworking' in reason for reason in result['reasons']))

    def test_odd_local_data_address_does_not_alias_previous_byte(self):
        original = reference(struct.pack('<I', 0x2001), extras=[symbol('_odd', 0x2001, section=2)],
                             extra_sections=[('__DATA', '__data', 0x2000, b'1234', 0, [])])
        reloc = {'type': 0, 'length': 2, 'address': 0, 'symbol': 2}
        def candidate(address):
            return macho([text(struct.pack('<I', address), relocations=[reloc]),
                          ('__DATA', '__data', 0x40, b'1234', 0, [])], [symbol('_probe'), symbol('_odd', 0x41, section=2)])
        self.assertEqual(self.comparison(original, candidate(0x41))['status'], 'matched')
        self.assertEqual(self.comparison(original, candidate(0x40))['status'], 'unresolved')
        placed = self.comparison(original, candidate(0x40), {'sections': {'__DATA,__data': 0x2000}})
        self.assertEqual(placed['status'], 'different')

    def test_data_in_code_annotations_are_used_on_both_sides(self):
        data = bytes.fromhex('0000a0e1') + ARM_BX
        original = macho([text(data, 0x1000)], executable_symbols([('_probe', 0x1000, 8, 'arm', 'unity')]),
                         filetype=2, data_ranges=[(28 + 56 + 68 + 24 + 16, 4, 1)])
        candidate = macho([text(data)], [symbol('_probe')], data_ranges=[(28 + 56 + 68 + 24 + 16, 4, 1)])
        result = self.comparison(original, candidate)
        self.assertEqual(result['status'], 'matched')
        for side in ('original', 'candidate'):
            self.assertEqual(result['rows'][0][side]['kind'], 'literal')
            self.assertEqual(result['rows'][0][side]['instruction'], '.word')

    def test_paired_movw_movt_arm_and_thumb(self):
        for thumb in (False, True):
            target = 0x12345679
            low, high = (0xf240, 0xf2c0) if thumb else (0xe3000000, 0xe3400000)
            original = struct.pack('<II', encode_half(low, target & 0xffff, thumb, False),
                                   encode_half(high, target >> 16, thumb, True))
            candidate = struct.pack('<II', low, high)
            relocs = []
            for offset, upper in ((0, False), (4, True)):
                relocs += [{'type': 8, 'length': int(upper) | (2 if thumb else 0), 'address': offset, 'external': True, 'symbol': 1},
                           {'type': 1, 'length': 0, 'address': 0}]
            r = self.comparison(reference(original, 'thumb' if thumb else 'arm', extras=[symbol('_target', target, type=2)]),
                                macho([text(candidate, relocations=relocs)], [symbol('_probe', desc=8 if thumb else 0), symbol('_target', section=0, type=1)]))
            self.assertEqual(r['status'], 'matched', r)
            self.assertEqual(len(r['relocations']), 2)
            r = self.comparison(reference(original, 'thumb' if thumb else 'arm'),
                                macho([text(candidate, relocations=relocs[:-1])], [symbol('_probe', desc=8 if thumb else 0), symbol('_target', section=0, type=1)]))
            self.assertEqual(r['status'], 'unresolved')

    def test_scattered_section_difference(self):
        original = reference(struct.pack('<I', 0x1000), extras=[symbol('_data', 0x2000, section=2)],
                             extra_sections=[('__DATA', '__data', 0x2000, b'1234', 0, [])])
        relocs = [{'type': 2, 'length': 2, 'address': 0, 'scattered': True, 'value': 0x40},
                  {'type': 1, 'length': 2, 'address': 0, 'scattered': True, 'value': 0}]
        candidate = macho([text(struct.pack('<I', 0x40), relocations=relocs),
                           ('__DATA', '__data', 0x40, b'1234', 0, [])], [symbol('_probe'), symbol('_data', 0x40, section=2)])
        r = self.comparison(original, candidate)
        self.assertEqual(r['status'], 'matched', r)
        self.assertEqual(r['relocations'][0]['subtract_address'], 0x1000)

    def test_local_section_relocation_requires_proven_placement(self):
        original = reference(struct.pack('<I', 0x2002), extra_sections=[('__DATA', '__data', 0x2000, b'1234', 0, [])])
        rel = {'type': 0, 'length': 2, 'address': 0, 'symbol': 2}
        candidate = macho([text(struct.pack('<I', 0x42), relocations=[rel]),
                           ('__DATA', '__data', 0x40, b'1234', 0, [])], [symbol('_probe')])
        self.assertEqual(self.comparison(original, candidate)['status'], 'unresolved')
        r = self.comparison(original, candidate, {'sections': {'__DATA,__data': 0x2000}})
        self.assertEqual(r['status'], 'matched')

    def test_duplicate_names_unity_groups_and_determinism(self):
        functions = [('_same', 0x1000, 4, 'arm', 'UnityA'), ('_same', 0x1004, 4, 'arm', 'UnityB')]
        binary = macho([text(ARM_BX * 2, 0x1000)], executable_symbols(functions), filetype=2)
        a, b = recover(MachO(binary)), recover(MachO(binary))
        self.assertEqual(a, b)
        self.assertEqual(a['coverage']['named_stabs_records'], 2)
        self.assertEqual(len({f['id'] for f in a['functions']}), 2)
        self.assertEqual([g['name'] for g in a['groups']], ['UnityA.o', 'UnityB.o'])

    def test_missing_size_and_overlapping_inventory_boundaries(self):
        funcs = [('_a', 0x1000, 8, 'arm', 'A'), ('_b', 0x1004, 4, 'arm', 'A')]
        m = MachO(macho([text(ARM_BX * 2, 0x1000)], executable_symbols(funcs), filetype=2))
        inv = recover(m)
        self.assertEqual(inv['coverage']['ambiguous_records'], 2)
        del m.symbols[4]  # remove first N_FUN ending size record
        inv = recover(m)
        self.assertEqual(inv['coverage']['named_stabs_records'], 2)
        self.assertTrue(inv['functions'][0]['ambiguities'])

    def test_malformed_macho_ranges_and_names(self):
        binary = macho([text(ARM_BX)], [symbol('_probe')])
        for malformed in [b'', binary[:27], binary[:-1], struct.pack('<I', 0xcafebabe) + binary[4:],
                          binary[:32] + struct.pack('<I', 0xffffffff) + binary[36:]]:
            with self.assertRaises(ToolError):
                MachO(malformed)
        bad = bytearray(binary)
        # section relocation count far outside file
        struct.pack_into('<I', bad, 28 + 56 + 52, 0xffffffff)
        with self.assertRaises(ToolError):
            MachO(bytes(bad))

    def test_mode_mismatch(self):
        r = self.comparison(reference(ARM_BX), macho([text(ARM_BX)], [symbol('_probe', desc=8)]))
        self.assertEqual(r['status'], 'unresolved')

    def test_literal_changes_are_visible_and_not_equal(self):
        prefix = bytes.fromhex('00009fe5') + ARM_BX
        a, b = prefix + struct.pack('<I', 7), prefix + struct.pack('<I', 9)
        r = self.comparison(reference(a), macho([text(b)], [symbol('_probe')]))
        self.assertEqual(r['status'], 'different')
        self.assertIn('operands', r['rows'][-1]['differences'])
