// Original compilation group o-f0f08bc53492515f7ed7.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-a3434231da3c3c1830ba — method-forwarder
// RevMapUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_a3434231da3c3c1830ba_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a3434231da3c3c1830ba(void * a0, void * a1, void * a2)
    __asm__("__ZN13RevMapUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a3434231da3c3c1830ba(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_a3434231da3c3c1830ba_target(a0, a1, a2);
}
