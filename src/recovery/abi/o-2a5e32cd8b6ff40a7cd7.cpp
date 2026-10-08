// Original compilation group o-2a5e32cd8b6ff40a7cd7.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-a3b1d6407fe4190f42d3 — complete-constructor
// ISE::TextureAnimation::TextureAnimation(ISE::TextureAnimation const&)
// Calls: ISE::TextureAnimation::TextureAnimation(ISE::TextureAnimation const&)
extern "C" void pirates_complete_constructor_a3b1d6407fe4190f42d3_target(void *, const void *)
    __asm__("__ZN3ISE16TextureAnimationC2ERKS0_");
extern "C" void pirates_complete_constructor_a3b1d6407fe4190f42d3(void * a0, const void * a1)
    __asm__("__ZN3ISE16TextureAnimationC1ERKS0_");
extern "C" void pirates_complete_constructor_a3b1d6407fe4190f42d3(void * a0, const void * a1) {
    pirates_complete_constructor_a3b1d6407fe4190f42d3_target(a0, a1);
}

// f-b23898f71696d83309f3 — complete-destructor
// ISE::TextureAnimation::~TextureAnimation()
// Calls: ISE::TextureAnimation::~TextureAnimation()
extern "C" void pirates_complete_destructor_b23898f71696d83309f3_target(void *)
    __asm__("__ZN3ISE16TextureAnimationD2Ev");
extern "C" void pirates_complete_destructor_b23898f71696d83309f3(void * a0)
    __asm__("__ZN3ISE16TextureAnimationD1Ev");
extern "C" void pirates_complete_destructor_b23898f71696d83309f3(void * a0) {
    pirates_complete_destructor_b23898f71696d83309f3_target(a0);
}
