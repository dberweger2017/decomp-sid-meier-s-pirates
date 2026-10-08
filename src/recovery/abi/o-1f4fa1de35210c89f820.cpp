// Original compilation group o-1f4fa1de35210c89f820.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-285a45737556752badef — method-forwarder
// ISE::ISEPSysSphereEmitter::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysVolumeEmitter::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_285a45737556752badef_target(void *, void *)
    __asm__("__ZN3ISE20ISEPSysVolumeEmitter10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_285a45737556752badef(void * a0, void * a1)
    __asm__("__ZN3ISE20ISEPSysSphereEmitter10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_285a45737556752badef(void * a0, void * a1) {
    pirates_method_forwarder_285a45737556752badef_target(a0, a1);
}
