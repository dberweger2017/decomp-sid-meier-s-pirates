import importlib.util
import struct
import unittest
from tools.battle_grid_probe import compare_execution, execute, GRID, environment
from tools.pirates.util import ToolError


@unittest.skipUnless(importlib.util.find_spec('unicorn'), 'optional hash-pinned ARM emulator is not installed')
class ArmExecutionTests(unittest.TestCase):
    def images(self, result=3, extra_nop=False):
        # Independently authored synthetic ARM: square calls property, writes its
        # return to memory, restores saved registers and returns. No game bytes.
        square, prop = 0x100000, 0x110000
        displacement = (prop - (square + 8 + 8)) >> 2
        code = struct.pack('<5I', 0xe92d4010, 0xe1a04000, 0xeb000000 | (displacement & 0xffffff), 0xe5840000, 0xe8bd8010)
        callee = struct.pack('<I', 0xe3a00000 | result)
        if extra_nop:
            callee += struct.pack('<I', 0xe1a00000)
        callee += struct.pack('<I', 0xe12fff1e)
        return {'square': (square, code), 'property': (prop, callee)}

    def cases(self):
        return [('synthetic-connected-call', [('square', [GRID])], environment(), None)]

    def test_connected_byte_different_candidate_can_pass_behavior(self):
        result = compare_execution(self.images(), self.images(extra_nop=True), self.cases())
        self.assertTrue(result[0]['passed'])
        self.assertNotEqual(self.images()['property'][1], self.images(extra_nop=True)['property'][1])

    def test_wrong_callee_effect_fails(self):
        result = compare_execution(self.images(), self.images(result=4), self.cases())
        self.assertFalse(result[0]['passed'])
        self.assertEqual(result[0]['differences'], ['memory'])

    def test_instruction_bound_failure_is_not_a_pass(self):
        images = self.images()
        images['square'] = (0x100000, struct.pack('<I', 0xeafffffe))
        with self.assertRaisesRegex(ToolError, 'instruction/time bound'):
            execute(images, self.cases()[0][1], environment())


if __name__ == '__main__':
    unittest.main()
