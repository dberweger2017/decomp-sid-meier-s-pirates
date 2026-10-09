// Original compilation group o-90d1fff07c5bef9ce409.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-dec61625089204d22548 — complete-constructor
// FontString::FontString(ISE::ISEColorA&, Font*)
// Calls: FontString::FontString(ISE::ISEColorA&, Font*)
extern "C" void pirates_complete_constructor_dec61625089204d22548_target(void *, void *, void *)
    __asm__("__ZN10FontStringC2ERN3ISE9ISEColorAEP4Font");
extern "C" void pirates_complete_constructor_dec61625089204d22548(void * a0, void * a1, void * a2)
    __asm__("__ZN10FontStringC1ERN3ISE9ISEColorAEP4Font");
extern "C" void pirates_complete_constructor_dec61625089204d22548(void * a0, void * a1, void * a2) {
    pirates_complete_constructor_dec61625089204d22548_target(a0, a1, a2);
}

// f-f69286ebb0d705d4f43b — complete-destructor
// FontString::~FontString()
// Calls: FontString::~FontString()
extern "C" void pirates_complete_destructor_f69286ebb0d705d4f43b_target(void *)
    __asm__("__ZN10FontStringD2Ev");
extern "C" void pirates_complete_destructor_f69286ebb0d705d4f43b(void * a0)
    __asm__("__ZN10FontStringD1Ev");
extern "C" void pirates_complete_destructor_f69286ebb0d705d4f43b(void * a0) {
    pirates_complete_destructor_f69286ebb0d705d4f43b_target(a0);
}
