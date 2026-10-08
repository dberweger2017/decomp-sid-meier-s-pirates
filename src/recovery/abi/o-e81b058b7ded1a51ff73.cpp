// Original compilation group o-e81b058b7ded1a51ff73.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-a84acdc82fa18ace27de — registration-forwarder
// _GLOBAL__I__ZN3ISE10ISEPosDataD2Ev
// Calls: ISE::ISEPosKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_a84acdc82fa18ace27de_target(void)
    __asm__("__ZN3ISE9ISEPosKey14RegisterLoaderEv");
static void pirates_registration_forwarder_a84acdc82fa18ace27de(void)
    __asm__("__GLOBAL__I__ZN3ISE10ISEPosDataD2Ev");
static void pirates_registration_forwarder_a84acdc82fa18ace27de(void) {
    pirates_registration_forwarder_a84acdc82fa18ace27de_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_a84acdc82fa18ace27de_source_reference)(void) = pirates_registration_forwarder_a84acdc82fa18ace27de;

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
