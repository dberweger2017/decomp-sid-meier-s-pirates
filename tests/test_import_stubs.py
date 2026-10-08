import struct
import unittest

from tests.fixtures import macho, text, symbol, executable_symbols, append_commands
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.compare import compare_function
from tools.pirates.util import ToolError


BX = bytes.fromhex('1eff2fe1')


def imported_image(names=('_import',), indices=None, opcode=0xe59fc000, stride=12):
    code = struct.pack('<I', 0xeb000000 | ((0x2000 - 0x1000 - 8) >> 2)) + BX
    symbols = executable_symbols([('_probe', 0x1000, 8, 'arm', 'unit')])
    start = len(symbols)
    symbols += [symbol(name, section=0, type=1) for name in names]
    stubs = b''.join(struct.pack('<III', opcode, 0xe59cf000, 0x3000 + i * 4)
                     for i in range(len(names)))
    data = bytearray(macho([text(code, 0x1000),
                           ('__TEXT', '__stubs', 0x2000, stubs, 0x80000408, []),
                           ('__DATA', '__la_symbol_ptr', 0x3000, bytes(4 * len(names)), 7, [])], symbols, filetype=2))
    struct.pack_into('<II', data, 28 + 56 + 68 + 60, 0, stride)
    struct.pack_into('<I', data, 28 + 56 + 136 + 60, len(names))
    indices = indices if indices is not None else list(range(start, start + len(names))) * 2
    fields = [0] * 18
    fields[12:14] = [len(data) + 80, len(indices)]
    return append_commands(bytes(data), [struct.pack('<20I', 11, 80, *fields)]) + struct.pack('<' + str(len(indices)) + 'I', *indices)


def compare(original, target='_import', reloc_type=5, encoded=0xebfffffe):
    rel = {'type': reloc_type, 'length': 2, 'pcrel': reloc_type == 5,
           'address': 0, 'external': True, 'symbol': 1}
    candidate = MachO(macho([text(struct.pack('<I', encoded) + BX, relocations=[rel])],
                            [symbol('_probe'), symbol(target, section=0, type=1)]))
    original = MachO(original)
    inventory = recover(original)
    return compare_function(original, inventory, inventory['functions'][0], candidate, details=True)


class ImportStubTests(unittest.TestCase):
    def test_indirect_arm_stub_resolves_branch_and_records_target(self):
        image = imported_image()
        self.assertEqual(MachO(image).imported_stubs,
                         [{'symbol': '_import', 'address': 0x2000, 'mode': 'arm'}])
        result = compare(image)
        self.assertEqual(result['status'], 'matched')
        self.assertEqual(result['relocations'][0]['target_address'], 0x2000)
        self.assertEqual(result['relocations'][0]['target'], '_import')

    def test_wrong_import_target_cannot_match(self):
        result = compare(imported_image(('_import', '_wrong')), '_wrong')
        self.assertEqual(result['status'], 'different')
        self.assertEqual(result['relocations'][0]['target_address'], 0x200c)

    def test_duplicate_stubs_and_nonzero_addends_are_unresolved(self):
        self.assertEqual(compare(imported_image(('_import', '_import')))['status'], 'unresolved')
        self.assertEqual(compare(imported_image(), encoded=0xebffffff)['status'], 'unresolved')

    def test_unsupported_encoding_or_pointer_name_mismatch_is_unresolved(self):
        self.assertEqual(compare(imported_image(opcode=0xe1a00000))['status'], 'unresolved')
        image = imported_image(('_import', '_wrong'))
        data = bytearray(image)
        # Swap the two lazy-pointer names while leaving stub names intact.
        a, b = struct.unpack_from('<II', data, len(data) - 8)
        struct.pack_into('<II', data, len(data) - 8, b, a)
        self.assertEqual(MachO(bytes(data)).imported_stubs, [])
        self.assertEqual(compare(bytes(data))['status'], 'unresolved')

    def test_stub_is_not_an_imported_function_pointer_address(self):
        self.assertEqual(compare(imported_image(), reloc_type=0, encoded=0)['status'], 'unresolved')

    def test_malformed_table_indices_ranges_and_strides_are_rejected(self):
        for image in (imported_image(indices=[0x3fffffff, 0x3fffffff]),
                      imported_image(indices=[]), imported_image(stride=0),
                      imported_image(stride=5), imported_image()[:-1]):
            with self.assertRaises(ToolError):
                MachO(image)
        data = bytearray(imported_image())
        pos = 28
        for _ in range(struct.unpack_from('<I', data, 16)[0]):
            command, size = struct.unpack_from('<II', data, pos)
            if command == 11:
                struct.pack_into('<I', data, pos + 8 + 12 * 4, 0xffffffff)
                break
            pos += size
        with self.assertRaises(ToolError):
            MachO(bytes(data))

    def test_local_and_absolute_indirect_markers_prove_no_import(self):
        image = imported_image(indices=[0x80000000, 0xc0000000])
        self.assertEqual(MachO(image).imported_stubs, [])
        self.assertEqual(compare(image)['status'], 'unresolved')
