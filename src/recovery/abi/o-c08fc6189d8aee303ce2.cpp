// Original compilation group o-c08fc6189d8aee303ce2.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-7632b6eb36a627cff2d7 — method-forwarder
// ISE::ISEParticleSystem::SetModelData(ISE::ISEParticleGeometryData*)
// Calls: ISE::ISEParticles::SetModelData(ISE::ISEParticleGeometryData*)
extern "C" void pirates_method_forwarder_7632b6eb36a627cff2d7_target(void *, void *)
    __asm__("__ZN3ISE12ISEParticles12SetModelDataEPNS_23ISEParticleGeometryDataE");
extern "C" void pirates_method_forwarder_7632b6eb36a627cff2d7(void * a0, void * a1)
    __asm__("__ZN3ISE17ISEParticleSystem12SetModelDataEPNS_23ISEParticleGeometryDataE");
extern "C" void pirates_method_forwarder_7632b6eb36a627cff2d7(void * a0, void * a1) {
    pirates_method_forwarder_7632b6eb36a627cff2d7_target(a0, a1);
}
