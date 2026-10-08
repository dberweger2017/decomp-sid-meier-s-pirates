// Original compilation group o-8c0a88f956f8a0a9caf0.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-501644445e4bb70f9653 — method-forwarder
// ISE::ISEFloatData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_501644445e4bb70f9653_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_501644445e4bb70f9653(void * a0, void * a1)
    __asm__("__ZN3ISE12ISEFloatData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_501644445e4bb70f9653(void * a0, void * a1) {
    pirates_method_forwarder_501644445e4bb70f9653_target(a0, a1);
}
