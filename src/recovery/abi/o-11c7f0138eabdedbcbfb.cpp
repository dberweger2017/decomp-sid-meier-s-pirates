// Original compilation group o-11c7f0138eabdedbcbfb.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-08a6eed34affa4fbf0cb — complete-destructor
// ISE::ISEParticleEntity::~ISEParticleEntity()
// Calls: ISE::ISEParticleEntity::~ISEParticleEntity()
extern "C" void pirates_complete_destructor_08a6eed34affa4fbf0cb_target(void *)
    __asm__("__ZN3ISE17ISEParticleEntityD2Ev");
extern "C" void pirates_complete_destructor_08a6eed34affa4fbf0cb(void * a0)
    __asm__("__ZN3ISE17ISEParticleEntityD1Ev");
extern "C" void pirates_complete_destructor_08a6eed34affa4fbf0cb(void * a0) {
    pirates_complete_destructor_08a6eed34affa4fbf0cb_target(a0);
}

// f-bf0967d7fdb81f248af6 — method-forwarder
// ISE::ISEParticleEntity::Render()
// Calls: ISE::ISERenderObject::Render()
extern "C" void pirates_method_forwarder_bf0967d7fdb81f248af6_target(void *)
    __asm__("__ZN3ISE15ISERenderObject6RenderEv");
extern "C" void pirates_method_forwarder_bf0967d7fdb81f248af6(void * a0)
    __asm__("__ZN3ISE17ISEParticleEntity6RenderEv");
extern "C" void pirates_method_forwarder_bf0967d7fdb81f248af6(void * a0) {
    pirates_method_forwarder_bf0967d7fdb81f248af6_target(a0);
}
