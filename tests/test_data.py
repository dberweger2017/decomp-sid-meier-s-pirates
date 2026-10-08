import copy
import hashlib
import json
import struct
import unittest
from tools.pirates.data import recover_data, compare_data
from tools.pirates.inventory import recover
from tools.pirates.macho import MachO
from tools.pirates.report import data_metrics, regression
from tools.pirates.util import ToolError
from tests.fixtures import reference, macho, symbol


def data_fixture(payload, zerofill=False, extra=(), offset=0):
    return MachO(reference(bytes.fromhex('1eff2fe1'), extra_sections=[
        ('__DATA', '__bss' if zerofill else '__data', 0x2000, b'\0' * offset + payload, 1 if zerofill else 0, [])],
        extras=[symbol('_table', 0x2000 + offset, section=2), *extra]))


def candidate(payload, zerofill=False, relocs=(), extra=(), offset=0):
    return MachO(macho([('__DATA', '__bss' if zerofill else '__data', 0, b'\0' * offset + payload,
                         1 if zerofill else 0, relocs)], [symbol('_table', offset), *extra]))


def compare(original, obj, details=True):
    inv = recover(original)
    record = next(d for d in recover_data(original, inv)['records'] if d['symbol'] == '_table')
    return compare_data(original, inv, record, obj, details=details)


class DataTests(unittest.TestCase):
    def test_full_allocation_includes_padding_and_missing_stays_visible(self):
        original = data_fixture(b'ab\0\0')
        self.assertEqual(compare(original, candidate(b'ab\0\0'))['status'], 'matched')
        self.assertEqual(compare(original, candidate(b'ab\0'))['status'], 'different')
        missing = compare(original, None)
        self.assertEqual(missing['status'], 'missing')
        self.assertEqual(missing['rows'][0]['original'], '61 62 00 00')
        self.assertEqual(data_metrics([missing])['matched_bytes'], 0)

    def test_literal_pools_are_not_counted_as_data(self):
        original = data_fixture(b'ab\0\0')
        inventory = recover_data(original, recover(original))
        self.assertEqual(inventory['coverage']['bytes'], 4)
        self.assertEqual(len(inventory['sections']), 1)
        self.assertEqual(inventory['sections'][0]['name'], '__DATA,__data')

    def test_resolved_pointer_and_wrong_target(self):
        original = data_fixture(struct.pack('<I', 0x3001), extra=[symbol('_target', 0x3001, section=0, type=2), symbol('_wrong', 0x4000, section=0, type=2)])
        rel = {'address': 0, 'type': 0, 'length': 2, 'external': True, 'symbol': 1}
        obj = candidate(b'\0' * 4, relocs=[rel], extra=[symbol('_target', section=0, type=1)])
        result = compare(original, obj)
        self.assertTrue(result['byte_verified'])
        self.assertEqual(result['relocations'][0]['target_address'], 0x3001)
        wrong = candidate(b'\0' * 4, relocs=[rel], extra=[symbol('_wrong', section=0, type=1)])
        self.assertEqual(compare(original, wrong)['status'], 'different')
        unknown = candidate(b'\0' * 4, relocs=[rel], extra=[symbol('_unknown', section=0, type=1)])
        self.assertEqual(compare(original, unknown)['status'], 'unresolved')

    def test_odd_data_start_is_not_thumb_tagged(self):
        original = data_fixture(b'abcd', offset=1)
        result = compare(original, candidate(b'abcd', offset=1))
        self.assertTrue(result['byte_verified'])
        self.assertEqual(result['rows'][0]['original'], '61 62 63 64')

    def test_scattered_section_difference_pointer_pair(self):
        original = data_fixture(struct.pack('<I', 0x1000), extra=[symbol('_target', 0x3000, section=0, type=2)])
        relocs = [{'address': 0, 'type': 2, 'length': 2, 'scattered': True, 'value': 0x100},
                  {'address': 0, 'type': 1, 'length': 2, 'scattered': True, 'value': 0}]
        obj = MachO(macho([('__DATA', '__data', 0, struct.pack('<I', 0x100), 0, relocs),
                           ('__TEXT', '__const', 0x100, b'abcd', 0, [])],
                          [symbol('_table'), symbol('_target', 0x100, section=2)]))
        result = compare(original, obj)
        self.assertTrue(result['byte_verified'])
        self.assertEqual(result['relocations'][0]['subtract_address'], 0x2000)

    def test_unsupported_relocation_never_verifies_even_equal_bytes(self):
        original = data_fixture(b'\0' * 4)
        rel = {'address': 0, 'type': 4, 'length': 2, 'external': True, 'symbol': 1}
        obj = candidate(b'\0' * 4, relocs=[rel], extra=[symbol('_target', type=2, section=0)])
        result = compare(original, obj)
        self.assertEqual(result['status'], 'unresolved')
        self.assertFalse(result['byte_verified'])

    def test_zero_fill_size_storage_and_alignment(self):
        original = data_fixture(b'\0' * 16, zerofill=True)
        obj = candidate(b'\0' * 16, zerofill=True)
        self.assertTrue(compare(original, obj)['byte_verified'])
        self.assertEqual(compare(original, candidate(b'\0' * 12, zerofill=True))['status'], 'different')
        self.assertEqual(compare(original, candidate(b'\0' * 16))['status'], 'unresolved')
        obj.alignments[1] = 8
        self.assertEqual(compare(original, obj)['status'], 'unresolved')

    def test_duplicate_names_aliases_and_malformed_ranges(self):
        original = data_fixture(b'abcd')
        obj = candidate(b'abcd', extra=[symbol('_alias')])
        self.assertEqual(compare(original, obj)['status'], 'unresolved')
        obj = candidate(b'abcd', extra=[symbol('_table', 2)])
        self.assertEqual(compare(original, obj)['status'], 'unresolved')
        original.symbols[-1].value = 0x9000
        with self.assertRaisesRegex(ToolError, 'outside section'):
            recover_data(original, recover(original))

    def test_data_match_and_coverage_regressions_fail(self):
        result = compare(data_fixture(b'abcd'), candidate(b'abcd'))
        base = {'kind': 'synthetic', 'input_sha256': 'x', 'inventory_sha256': 'y', 'profile': {}, 'compiler': {},
                'coverage': {}, 'functions': [], 'units': [], 'data': [result]}
        head = copy.deepcopy(base)
        head['data'][0].update(status='different', byte_verified=False)
        self.assertTrue(any('Verified data match regressed' in f for f in regression(base, head)['failures']))
        head['data'] = []
        self.assertTrue(any('Data inventory coverage disappeared' in f for f in regression(base, head)['failures']))

    def test_data_report_determinism(self):
        original = data_fixture(b'abcd')
        value = {'inventory': recover_data(original, recover(original)), 'comparison': compare(original, candidate(b'abcd'))}
        serialized = json.dumps(value, sort_keys=True, separators=(',', ':'))
        self.assertEqual(hashlib.sha256(serialized.encode()).hexdigest(), 'e9d344d6c5915166a297f33e34966a48c5fcebccbf25ec7b78e4ff05f3853835')
