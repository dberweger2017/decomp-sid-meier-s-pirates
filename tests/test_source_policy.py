import unittest

from tools.pirates.source_policy import validate_source
from tools.pirates.util import ToolError


class SourcePolicyTests(unittest.TestCase):
    def test_rejects_file_scope_payloads_and_instruction_bodies(self):
        for source in (
                '__asm__(".text\\n.globl _probe\\n_probe:\\n.word 0xe12fff1e\\n");',
                'void probe() { __asm__ volatile(".byte 0x1e,0xff,0x2f,0xe1"); }',
                'void probe() { asm("bx lr"); }',
                'void probe() { __asm("nop"); }',
                '__asm__(".incbin \\\"original.bin\\\"");',
                'void probe() { if (1) asm("nop"); }',
                'void probe() { if (1) {} else asm("nop"); }',
                'void probe() { __extension__ asm("nop"); }',
                'void probe() { asm("" : : : "memory"); }',
                '__asm__("\\x2e\\x77ord 0xe12fff1e");',
                'void probe() { asm goto("b %l0" : : : : done); done:; }'):
            with self.subTest(source=source), self.assertRaisesRegex(ToolError, 'assembly statements'):
                validate_source(source)

    def test_preserves_abi_declarations_and_register_bindings(self):
        validate_source('''
extern "C" void target(void *) __asm__("__ZN5ProbeD2Ev");
extern "C" void entry(void *a) __asm__("_probe");
extern "C" void entry(void *a) { target(a); }
int getrlimit(int, void *) __asm("_" "getrlimit" "$UNIX2003");
static unsigned char rtti[8] __asm__("__ZN5Probe6m_RTTIE") __attribute__((aligned(16)));
void register_binding() { register int stride asm("r0") = 32; }
const int table[2] __attribute__((section("__TEXT,__const"))) = {1, 2};
''')

    def test_comments_and_ordinary_strings_are_not_assembly(self):
        validate_source('''
// __asm__(".word 0xe12fff1e");
/* asm volatile(".incbin original.bin"); */
const char *note = "__asm__(\\".word 0xe12fff1e\\");";
const char *raw = R"note(asm(".word 0xe12fff1e"))note";
int probe(int x) { return x + 41; }
''')

    def test_rejects_arrays_placed_in_executable_sections(self):
        for attribute in (
                'section("__TEXT,__text")',
                '__section__("__TEXT, __textcoal_nt")',
                'section("__TEXT,__payload,regular,pure_instructions")',
                'section("__TEXT,__payload,regular,some_instructions")',
                'section("__TEXT," "__text")',
                'section("__TEXT,\\x5f\\x5ftext")'):
            with self.subTest(attribute=attribute), self.assertRaisesRegex(ToolError, 'executable-section'):
                validate_source('const unsigned code[] __attribute__((' + attribute + ')) = {0xe12fff1e};')

    def test_reports_original_include_location_from_preprocessor(self):
        source = '# 1 "src/unity.cpp"\n# 17 "src/payload.h" 1\n\n__asm__(".word 0xe12fff1e");\n'
        with self.assertRaisesRegex(ToolError, r'src/payload.h:18'):
            validate_source(source)
