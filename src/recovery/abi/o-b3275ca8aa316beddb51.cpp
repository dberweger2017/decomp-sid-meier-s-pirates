// Original compilation group o-b3275ca8aa316beddb51.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-664115e77d1276d17a7c — complete-destructor
// ISE::ISEParticlesData::~ISEParticlesData()
// Calls: ISE::ISEParticlesData::~ISEParticlesData()
extern "C" void pirates_complete_destructor_664115e77d1276d17a7c_target(void *)
    __asm__("__ZN3ISE16ISEParticlesDataD2Ev");
extern "C" void pirates_complete_destructor_664115e77d1276d17a7c(void * a0)
    __asm__("__ZN3ISE16ISEParticlesDataD1Ev");
extern "C" void pirates_complete_destructor_664115e77d1276d17a7c(void * a0) {
    pirates_complete_destructor_664115e77d1276d17a7c_target(a0);
}

// f-c47528c16e2132c51db3 — method-forwarder
// ISE::ISEParticlesData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleGeometryData::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_c47528c16e2132c51db3_target(void *, void *)
    __asm__("__ZN3ISE23ISEParticleGeometryData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_c47528c16e2132c51db3(void * a0, void * a1)
    __asm__("__ZN3ISE16ISEParticlesData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_c47528c16e2132c51db3(void * a0, void * a1) {
    pirates_method_forwarder_c47528c16e2132c51db3_target(a0, a1);
}
