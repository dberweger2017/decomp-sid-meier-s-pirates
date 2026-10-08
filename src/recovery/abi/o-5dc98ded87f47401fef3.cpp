// Original compilation group o-5dc98ded87f47401fef3.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-7171567d214deff67792 — method-forwarder
// ISE::ISEPSysEmitterInitialRadiusCtlr::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_7171567d214deff67792_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_7171567d214deff67792(void * a0, void * a1)
    __asm__("__ZN3ISE31ISEPSysEmitterInitialRadiusCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_7171567d214deff67792(void * a0, void * a1) {
    pirates_method_forwarder_7171567d214deff67792_target(a0, a1);
}

// f-787d29af18520b2790a1 — method-forwarder
// ISE::ISEPSysEmitterInitialRadiusCtlr::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_787d29af18520b2790a1_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_787d29af18520b2790a1(void * a0, void * a1)
    __asm__("__ZN3ISE31ISEPSysEmitterInitialRadiusCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_787d29af18520b2790a1(void * a0, void * a1) {
    pirates_method_forwarder_787d29af18520b2790a1_target(a0, a1);
}

// f-a6c35ae2fb2e854e19cf — registration-forwarder
// _GLOBAL__I__ZN3ISE31ISEPSysEmitterInitialRadiusCtlrC2EPKcPNS_12ISEFloatDataE
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_a6c35ae2fb2e854e19cf_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_a6c35ae2fb2e854e19cf(void)
    __asm__("__GLOBAL__I__ZN3ISE31ISEPSysEmitterInitialRadiusCtlrC2EPKcPNS_12ISEFloatDataE");
static void pirates_registration_forwarder_a6c35ae2fb2e854e19cf(void) {
    pirates_registration_forwarder_a6c35ae2fb2e854e19cf_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_a6c35ae2fb2e854e19cf_source_reference)(void) = pirates_registration_forwarder_a6c35ae2fb2e854e19cf;
