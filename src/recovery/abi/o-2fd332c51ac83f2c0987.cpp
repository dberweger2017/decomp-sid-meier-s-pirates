// Original compilation group o-2fd332c51ac83f2c0987.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-a66f63554cda2b95d7f9 — method-forwarder
// MultiPlaySBChosenUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_a66f63554cda2b95d7f9_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a66f63554cda2b95d7f9(void * a0, void * a1, void * a2)
    __asm__("__ZN24MultiPlaySBChosenUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a66f63554cda2b95d7f9(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_a66f63554cda2b95d7f9_target(a0, a1, a2);
}
