// Original compilation group o-22e518abce9579acfc03.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-fd176811eeb2f6a6da87 — registration-forwarder
// _GLOBAL__I__ZN3ISE12ISEParticlesC2EtP8PVRTVec3S2_PNS_9ISEColorAEP15PVRTQUATERNIONf
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_fd176811eeb2f6a6da87_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_fd176811eeb2f6a6da87(void)
    __asm__("__GLOBAL__I__ZN3ISE12ISEParticlesC2EtP8PVRTVec3S2_PNS_9ISEColorAEP15PVRTQUATERNIONf");
static void pirates_registration_forwarder_fd176811eeb2f6a6da87(void) {
    pirates_registration_forwarder_fd176811eeb2f6a6da87_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_fd176811eeb2f6a6da87_source_reference)(void) = pirates_registration_forwarder_fd176811eeb2f6a6da87;

// f-fdf6117e6371a24afc9a — complete-destructor
// ISE::ISEParticles::~ISEParticles()
// Calls: ISE::ISEParticles::~ISEParticles()
extern "C" void pirates_complete_destructor_fdf6117e6371a24afc9a_target(void *)
    __asm__("__ZN3ISE12ISEParticlesD2Ev");
extern "C" void pirates_complete_destructor_fdf6117e6371a24afc9a(void * a0)
    __asm__("__ZN3ISE12ISEParticlesD1Ev");
extern "C" void pirates_complete_destructor_fdf6117e6371a24afc9a(void * a0) {
    pirates_complete_destructor_fdf6117e6371a24afc9a_target(a0);
}
