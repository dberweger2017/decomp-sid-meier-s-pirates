// Original compilation group o-b5a90cc041122ccd1748.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-c58938cbd51e68f04bda — method-forwarder
// UicFrameAnimation::Update(PVRTVec2*, PVRTVec2*)
// Calls: UicFrameAnimation::updateAnimation()
extern "C" void pirates_method_forwarder_c58938cbd51e68f04bda_target(void *)
    __asm__("__ZN17UicFrameAnimation15updateAnimationEv");
extern "C" void pirates_method_forwarder_c58938cbd51e68f04bda(void * a0, void * a1, void * a2)
    __asm__("__ZN17UicFrameAnimation6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_c58938cbd51e68f04bda(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_c58938cbd51e68f04bda_target(a0);
}

// f-e8b74efa90ef27bb908b — complete-destructor
// UicFrameAnimation::~UicFrameAnimation()
// Calls: UicFrameAnimation::~UicFrameAnimation()
extern "C" void pirates_complete_destructor_e8b74efa90ef27bb908b_target(void *)
    __asm__("__ZN17UicFrameAnimationD2Ev");
extern "C" void pirates_complete_destructor_e8b74efa90ef27bb908b(void * a0)
    __asm__("__ZN17UicFrameAnimationD1Ev");
extern "C" void pirates_complete_destructor_e8b74efa90ef27bb908b(void * a0) {
    pirates_complete_destructor_e8b74efa90ef27bb908b_target(a0);
}
