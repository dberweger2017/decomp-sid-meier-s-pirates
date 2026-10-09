// Original compilation group o-314a424024475377a06a.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-36c5fd7a90a39b78ac69 — registration-forwarder
// _GLOBAL__I__ZN3ISE29ISEPSysEmitterPlanarAngleCtlrC2Ev
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_36c5fd7a90a39b78ac69_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_36c5fd7a90a39b78ac69(void)
    __asm__("__GLOBAL__I__ZN3ISE29ISEPSysEmitterPlanarAngleCtlrC2Ev");
static void pirates_registration_forwarder_36c5fd7a90a39b78ac69(void) {
    pirates_registration_forwarder_36c5fd7a90a39b78ac69_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_36c5fd7a90a39b78ac69_source_reference)(void) = pirates_registration_forwarder_36c5fd7a90a39b78ac69;

// f-4dc15c090b66376e2808 — method-forwarder
// ISE::ISEPSysEmitterPlanarAngleCtlr::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_4dc15c090b66376e2808_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_4dc15c090b66376e2808(void * a0, void * a1)
    __asm__("__ZN3ISE29ISEPSysEmitterPlanarAngleCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_4dc15c090b66376e2808(void * a0, void * a1) {
    pirates_method_forwarder_4dc15c090b66376e2808_target(a0, a1);
}

// f-c8245f2220603ecd3c4f — method-forwarder
// ISE::ISEPSysEmitterPlanarAngleCtlr::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_c8245f2220603ecd3c4f_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_c8245f2220603ecd3c4f(void * a0, void * a1)
    __asm__("__ZN3ISE29ISEPSysEmitterPlanarAngleCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_c8245f2220603ecd3c4f(void * a0, void * a1) {
    pirates_method_forwarder_c8245f2220603ecd3c4f_target(a0, a1);
}
