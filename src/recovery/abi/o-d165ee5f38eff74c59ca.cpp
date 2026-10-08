// Original compilation group o-d165ee5f38eff74c59ca.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-0c29fd2f905390f47626 — method-forwarder
// FAudioSystemPhono::GetLowLevelSoundObject(void*)
// Calls: FPhono::GetDirectSoundObject()
extern "C" void * pirates_method_forwarder_0c29fd2f905390f47626_target(void)
    __asm__("__ZN6FPhono20GetDirectSoundObjectEv");
extern "C" void * pirates_method_forwarder_0c29fd2f905390f47626(void * a0, void * a1)
    __asm__("__ZN17FAudioSystemPhono22GetLowLevelSoundObjectEPv");
extern "C" void * pirates_method_forwarder_0c29fd2f905390f47626(void * a0, void * a1) {
    return pirates_method_forwarder_0c29fd2f905390f47626_target();
}

// f-e104097d5d6d00d6eba4 — complete-destructor
// FAudioSystemPhono::~FAudioSystemPhono()
// Calls: FAudioSystemPhono::~FAudioSystemPhono()
extern "C" void pirates_complete_destructor_e104097d5d6d00d6eba4_target(void *)
    __asm__("__ZN17FAudioSystemPhonoD2Ev");
extern "C" void pirates_complete_destructor_e104097d5d6d00d6eba4(void * a0)
    __asm__("__ZN17FAudioSystemPhonoD1Ev");
extern "C" void pirates_complete_destructor_e104097d5d6d00d6eba4(void * a0) {
    pirates_complete_destructor_e104097d5d6d00d6eba4_target(a0);
}
