// Original compilation group o-12bfeeb050e378230efa.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-f9bd2ab8335f94e8282e — method-forwarder
// ISE::ISEPSysData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticlesData::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_f9bd2ab8335f94e8282e_target(void *, void *)
    __asm__("__ZN3ISE16ISEParticlesData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_f9bd2ab8335f94e8282e(void * a0, void * a1)
    __asm__("__ZN3ISE11ISEPSysData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_f9bd2ab8335f94e8282e(void * a0, void * a1) {
    pirates_method_forwarder_f9bd2ab8335f94e8282e_target(a0, a1);
}
