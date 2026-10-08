// Original compilation group o-7d25f6180d9844d7c020.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-0f91739920084c5304f9 — function-forwarder
// NiStricmp(char const*, char const*)
// Calls: stricmp(char const*, char const*)
extern "C" int pirates_function_forwarder_0f91739920084c5304f9_target(const void *, const void *)
    __asm__("__Z7stricmpPKcS0_");
extern "C" int pirates_function_forwarder_0f91739920084c5304f9(const void * a0, const void * a1)
    __asm__("__Z9NiStricmpPKcS0_");
extern "C" int pirates_function_forwarder_0f91739920084c5304f9(const void * a0, const void * a1) {
    return pirates_function_forwarder_0f91739920084c5304f9_target(a0, a1);
}

// f-17b70a210b8d350813c4 — complete-destructor
// NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::~NiTMapBase()
// Calls: NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::~NiTMapBase()
extern "C" void pirates_complete_destructor_17b70a210b8d350813c4_target(void *)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEP8NiObjectS3_ED2Ev");
extern "C" void pirates_complete_destructor_17b70a210b8d350813c4(void * a0)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEP8NiObjectS3_ED1Ev");
extern "C" void pirates_complete_destructor_17b70a210b8d350813c4(void * a0) {
    pirates_complete_destructor_17b70a210b8d350813c4_target(a0);
}

// f-1ebfc745d826666e0edb — function-forwarder
// OS_timeGetTime()
// Calls: IpTimeRead()
extern "C" unsigned int pirates_function_forwarder_1ebfc745d826666e0edb_target(void)
    __asm__("__Z10IpTimeReadv");
extern "C" unsigned int pirates_function_forwarder_1ebfc745d826666e0edb(void)
    __asm__("__Z14OS_timeGetTimev");
extern "C" unsigned int pirates_function_forwarder_1ebfc745d826666e0edb(void) {
    return pirates_function_forwarder_1ebfc745d826666e0edb_target();
}

// f-8f42fa828b34c70f360c — function-forwarder
// OS_Sleep(unsigned int)
// Calls: Phono2::Sleep(int)
extern "C" void pirates_function_forwarder_8f42fa828b34c70f360c_target(int)
    __asm__("__ZN6Phono25SleepEi");
extern "C" void pirates_function_forwarder_8f42fa828b34c70f360c(unsigned int a0)
    __asm__("__Z8OS_Sleepj");
extern "C" void pirates_function_forwarder_8f42fa828b34c70f360c(unsigned int a0) {
    pirates_function_forwarder_8f42fa828b34c70f360c_target(a0);
}

// f-bc5af2caeedf7941a3cb — complete-destructor
// NiPick::~NiPick()
// Calls: NiPick::~NiPick()
extern "C" void pirates_complete_destructor_bc5af2caeedf7941a3cb_target(void *)
    __asm__("__ZN6NiPickD2Ev");
extern "C" void pirates_complete_destructor_bc5af2caeedf7941a3cb(void * a0)
    __asm__("__ZN6NiPickD1Ev");
extern "C" void pirates_complete_destructor_bc5af2caeedf7941a3cb(void * a0) {
    pirates_complete_destructor_bc5af2caeedf7941a3cb_target(a0);
}

// f-f3b4d0cbdc9568b63d7e — complete-destructor
// NiTPointerMap<NiObject*, NiObject*>::~NiTPointerMap()
// Calls: NiTPointerMap<NiObject*, NiObject*>::~NiTPointerMap()
extern "C" void pirates_complete_destructor_f3b4d0cbdc9568b63d7e_target(void *)
    __asm__("__ZN13NiTPointerMapIP8NiObjectS1_ED2Ev");
extern "C" void pirates_complete_destructor_f3b4d0cbdc9568b63d7e(void * a0)
    __asm__("__ZN13NiTPointerMapIP8NiObjectS1_ED1Ev");
extern "C" void pirates_complete_destructor_f3b4d0cbdc9568b63d7e(void * a0) {
    pirates_complete_destructor_f3b4d0cbdc9568b63d7e_target(a0);
}

// f-fda4f8b1db2d7520b0c6 — complete-destructor
// Pvr_NiTexture::~Pvr_NiTexture()
// Calls: Pvr_NiTexture::~Pvr_NiTexture()
extern "C" void pirates_complete_destructor_fda4f8b1db2d7520b0c6_target(void *)
    __asm__("__ZN13Pvr_NiTextureD2Ev");
extern "C" void pirates_complete_destructor_fda4f8b1db2d7520b0c6(void * a0)
    __asm__("__ZN13Pvr_NiTextureD1Ev");
extern "C" void pirates_complete_destructor_fda4f8b1db2d7520b0c6(void * a0) {
    pirates_complete_destructor_fda4f8b1db2d7520b0c6_target(a0);
}
