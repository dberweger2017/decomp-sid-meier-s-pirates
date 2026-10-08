// Original compilation group o-fb44be03218619565a91.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-ad9484c8da1588222e94 — method-forwarder
// ISE::ISEPSysSpawnModifier::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifier::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_ad9484c8da1588222e94_target(void *, void *)
    __asm__("__ZN3ISE15ISEPSysModifier10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_ad9484c8da1588222e94(void * a0, void * a1)
    __asm__("__ZN3ISE20ISEPSysSpawnModifier10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_ad9484c8da1588222e94(void * a0, void * a1) {
    pirates_method_forwarder_ad9484c8da1588222e94_target(a0, a1);
}
