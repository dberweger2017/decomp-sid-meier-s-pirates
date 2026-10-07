import hashlib
import json
import unittest
from tests.fixtures import reference, macho, text, symbol
from tools.pirates.macho import MachO
from tools.pirates.inventory import recover
from tools.pirates.compare import compare_function
from tools.pirates.report import native_report, objdiff_adapter


def canonical_fixture_report():
    original = MachO(reference(bytes.fromhex('00009fe51eff2fe101200000'), extras=[symbol('_literal', 0x2001, type=2)]))
    inv = recover(original)
    r = {'type': 0, 'length': 2, 'address': 8, 'external': True, 'symbol': 1}
    candidate = MachO(macho([text(bytes.fromhex('00009fe51eff2fe100000000'), relocations=[r])],
                            [symbol('_probe'), symbol('_literal', type=1, section=0)]))
    f = inv['functions'][0]
    result = compare_function(original, inv, f, candidate, details=True)
    result.update({k: f[k] for k in ('size', 'section', 'group_id', 'symbol', 'address', 'mode', 'source_path')})
    result['section_offset'] = 0
    return {'inventory': inv, 'comparison': result}


class DeterminismTests(unittest.TestCase):
    def test_fixed_core_report_hash_on_every_supported_host(self):
        # A fixed independently encoded input exercises parsing, literal pools,
        # relocation targets, ARM operands and deterministic serialization. CI
        # checks this same digest on macOS and Linux, independent of host Clang.
        actual = hashlib.sha256(json.dumps(canonical_fixture_report(), sort_keys=True, separators=(',', ':')).encode()).hexdigest()
        self.assertEqual(actual, '3780cbbf45b27aaea7ed164a3d024c276935a27f8cae1941d4dc21edadfda4dd')
