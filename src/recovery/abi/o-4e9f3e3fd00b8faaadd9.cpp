// Original compilation group o-4e9f3e3fd00b8faaadd9.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-704c17586b87a47e44c8 — method-forwarder
// ISE::ISEPSysCylinderEmitter::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysVolumeEmitter::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_704c17586b87a47e44c8_target(void *, void *)
    __asm__("__ZN3ISE20ISEPSysVolumeEmitter10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_704c17586b87a47e44c8(void * a0, void * a1)
    __asm__("__ZN3ISE22ISEPSysCylinderEmitter10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_704c17586b87a47e44c8(void * a0, void * a1) {
    pirates_method_forwarder_704c17586b87a47e44c8_target(a0, a1);
}
