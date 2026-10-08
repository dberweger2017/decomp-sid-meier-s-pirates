// Original compilation group o-615f11e1c3ff7aa6b275.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-b580462ea59e3b6cb0ff — method-forwarder
// MultiPlaySFCharacterChosenUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_b580462ea59e3b6cb0ff_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_b580462ea59e3b6cb0ff(void * a0, void * a1, void * a2)
    __asm__("__ZN33MultiPlaySFCharacterChosenUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_b580462ea59e3b6cb0ff(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_b580462ea59e3b6cb0ff_target(a0, a1, a2);
}
