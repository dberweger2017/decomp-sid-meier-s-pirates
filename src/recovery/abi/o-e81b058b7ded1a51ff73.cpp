// Original compilation group o-e81b058b7ded1a51ff73.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-e5ea3d669952dfcc8979 — method-forwarder
// ISE::ISEPosData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_e5ea3d669952dfcc8979_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_e5ea3d669952dfcc8979(void * a0, void * a1)
    __asm__("__ZN3ISE10ISEPosData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_e5ea3d669952dfcc8979(void * a0, void * a1) {
    pirates_method_forwarder_e5ea3d669952dfcc8979_target(a0, a1);
}
