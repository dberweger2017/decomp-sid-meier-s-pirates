// Original compilation group o-feef30570e6ce15967c0.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-0a8d73a2b9277a3420d7 — method-forwarder
// ISE::ISEPSysEmitterDeclinationCtlr::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_0a8d73a2b9277a3420d7_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_0a8d73a2b9277a3420d7(void * a0, void * a1)
    __asm__("__ZN3ISE29ISEPSysEmitterDeclinationCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_0a8d73a2b9277a3420d7(void * a0, void * a1) {
    pirates_method_forwarder_0a8d73a2b9277a3420d7_target(a0, a1);
}

// f-add508fdd266fc3261a4 — method-forwarder
// ISE::ISEPSysEmitterDeclinationCtlr::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEPSysModifierFloatCtlr::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_add508fdd266fc3261a4_target(void *, void *)
    __asm__("__ZN3ISE24ISEPSysModifierFloatCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_add508fdd266fc3261a4(void * a0, void * a1)
    __asm__("__ZN3ISE29ISEPSysEmitterDeclinationCtlr10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_add508fdd266fc3261a4(void * a0, void * a1) {
    pirates_method_forwarder_add508fdd266fc3261a4_target(a0, a1);
}

// f-f1906bdd5ba8335a0345 — registration-forwarder
// _GLOBAL__I__ZN3ISE29ISEPSysEmitterDeclinationCtlrC2Ev
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_f1906bdd5ba8335a0345_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_f1906bdd5ba8335a0345(void)
    __asm__("__GLOBAL__I__ZN3ISE29ISEPSysEmitterDeclinationCtlrC2Ev");
static void pirates_registration_forwarder_f1906bdd5ba8335a0345(void) {
    pirates_registration_forwarder_f1906bdd5ba8335a0345_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_f1906bdd5ba8335a0345_source_reference)(void) = pirates_registration_forwarder_f1906bdd5ba8335a0345;
