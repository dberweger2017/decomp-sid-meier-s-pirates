// Original compilation group o-d97284a3dcb35bd26b91.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-0292f1210e157e376c35 — method-forwarder
// MultiTwoUIScene::Update(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Update(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_0292f1210e157e376c35_target(void *, void *, void *)
    __asm__("__ZN7UIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_0292f1210e157e376c35(void * a0, void * a1, void * a2)
    __asm__("__ZN15MultiTwoUIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_0292f1210e157e376c35(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_0292f1210e157e376c35_target(a0, a1, a2);
}

// f-b999ab7805eb8ecd1dd2 — method-forwarder
// MultiTwoUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_b999ab7805eb8ecd1dd2_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_b999ab7805eb8ecd1dd2(void * a0, void * a1, void * a2)
    __asm__("__ZN15MultiTwoUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_b999ab7805eb8ecd1dd2(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_b999ab7805eb8ecd1dd2_target(a0, a1, a2);
}
