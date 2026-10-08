// Original compilation group o-8c0a88f956f8a0a9caf0.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-10cacc625beb0c42cb04 — registration-forwarder
// _GLOBAL__I__ZN3ISE12ISEFloatDataD2Ev
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_10cacc625beb0c42cb04_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_10cacc625beb0c42cb04(void)
    __asm__("__GLOBAL__I__ZN3ISE12ISEFloatDataD2Ev");
static void pirates_registration_forwarder_10cacc625beb0c42cb04(void) {
    pirates_registration_forwarder_10cacc625beb0c42cb04_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_10cacc625beb0c42cb04_source_reference)(void) = pirates_registration_forwarder_10cacc625beb0c42cb04;

// f-501644445e4bb70f9653 — method-forwarder
// ISE::ISEFloatData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_501644445e4bb70f9653_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_501644445e4bb70f9653(void * a0, void * a1)
    __asm__("__ZN3ISE12ISEFloatData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_501644445e4bb70f9653(void * a0, void * a1) {
    pirates_method_forwarder_501644445e4bb70f9653_target(a0, a1);
}
