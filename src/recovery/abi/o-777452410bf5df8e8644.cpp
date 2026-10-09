// Original compilation group o-777452410bf5df8e8644.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-504a7a8f76761c9ea969 — method-forwarder
// ISE::ISEPSysEmitterSpeedCtlr::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_504a7a8f76761c9ea969_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_504a7a8f76761c9ea969(void * a0, void * a1)
    __asm__("__ZN3ISE23ISEPSysEmitterSpeedCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_504a7a8f76761c9ea969(void * a0, void * a1) {
    pirates_method_forwarder_504a7a8f76761c9ea969_target(a0, a1);
}

// f-74fb71cf0948aa199802 — method-forwarder
// ISE::ISEPSysEmitterSpeedCtlr::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_74fb71cf0948aa199802_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_74fb71cf0948aa199802(void * a0, void * a1)
    __asm__("__ZN3ISE23ISEPSysEmitterSpeedCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_74fb71cf0948aa199802(void * a0, void * a1) {
    pirates_method_forwarder_74fb71cf0948aa199802_target(a0, a1);
}

// f-ed736ac1b35e7c2971f8 — registration-forwarder
// _GLOBAL__I__ZN3ISE23ISEPSysEmitterSpeedCtlrC2EPKcPNS_12ISEFloatDataE
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_ed736ac1b35e7c2971f8_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_ed736ac1b35e7c2971f8(void)
    __asm__("__GLOBAL__I__ZN3ISE23ISEPSysEmitterSpeedCtlrC2EPKcPNS_12ISEFloatDataE");
static void pirates_registration_forwarder_ed736ac1b35e7c2971f8(void) {
    pirates_registration_forwarder_ed736ac1b35e7c2971f8_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_ed736ac1b35e7c2971f8_source_reference)(void) = pirates_registration_forwarder_ed736ac1b35e7c2971f8;
