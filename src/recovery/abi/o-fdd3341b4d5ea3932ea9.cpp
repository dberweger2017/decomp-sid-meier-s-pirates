// Original compilation group o-fdd3341b4d5ea3932ea9.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-a306d36fa68ae1dfc0c4 — method-forwarder
// ISE::ISEParticleColorData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_a306d36fa68ae1dfc0c4_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_a306d36fa68ae1dfc0c4(void * a0, void * a1)
    __asm__("__ZN3ISE20ISEParticleColorData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_a306d36fa68ae1dfc0c4(void * a0, void * a1) {
    pirates_method_forwarder_a306d36fa68ae1dfc0c4_target(a0, a1);
}
