// Original compilation group o-f6f13f045c159771f24a.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-96d5caa343650a3079f8 — method-forwarder
// ISE::ISEVisData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_96d5caa343650a3079f8_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_96d5caa343650a3079f8(void * a0, void * a1)
    __asm__("__ZN3ISE10ISEVisData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_96d5caa343650a3079f8(void * a0, void * a1) {
    pirates_method_forwarder_96d5caa343650a3079f8_target(a0, a1);
}
