// Original compilation group o-0de26449ca836468bf0d.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-441d9d9ec3880f96ef80 — method-forwarder
// ISE::ISEPSysGrowFadeModifier::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifier::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_441d9d9ec3880f96ef80_target(void *, void *)
    __asm__("__ZN3ISE15ISEPSysModifier10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_441d9d9ec3880f96ef80(void * a0, void * a1)
    __asm__("__ZN3ISE23ISEPSysGrowFadeModifier10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_441d9d9ec3880f96ef80(void * a0, void * a1) {
    pirates_method_forwarder_441d9d9ec3880f96ef80_target(a0, a1);
}
