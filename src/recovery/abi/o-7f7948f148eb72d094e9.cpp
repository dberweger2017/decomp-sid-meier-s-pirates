// Original compilation group o-7f7948f148eb72d094e9.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-72005730178224ad7cd9 — method-forwarder
// CoverUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_72005730178224ad7cd9_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_72005730178224ad7cd9(void * a0, void * a1, void * a2)
    __asm__("__ZN12CoverUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_72005730178224ad7cd9(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_72005730178224ad7cd9_target(a0, a1, a2);
}
