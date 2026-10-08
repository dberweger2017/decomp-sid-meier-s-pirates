// Original compilation group o-5fe61a2115ec66b4e06f.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-f4ac95ca94d9a3cf2354 — method-forwarder
// ISE::ISEPSysPositionModifier::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifier::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_f4ac95ca94d9a3cf2354_target(void *, void *)
    __asm__("__ZN3ISE15ISEPSysModifier10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_f4ac95ca94d9a3cf2354(void * a0, void * a1)
    __asm__("__ZN3ISE23ISEPSysPositionModifier10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_f4ac95ca94d9a3cf2354(void * a0, void * a1) {
    pirates_method_forwarder_f4ac95ca94d9a3cf2354_target(a0, a1);
}
