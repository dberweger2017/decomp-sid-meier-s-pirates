// Original compilation group o-aa2cd45009f7b58b8aa4.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-074d802cedbbaa1e9784 — complete-destructor
// TradeUIScene::~TradeUIScene()
// Calls: TradeUIScene::~TradeUIScene()
extern "C" void pirates_complete_destructor_074d802cedbbaa1e9784_target(void *)
    __asm__("__ZN12TradeUISceneD2Ev");
extern "C" void pirates_complete_destructor_074d802cedbbaa1e9784(void * a0)
    __asm__("__ZN12TradeUISceneD1Ev");
extern "C" void pirates_complete_destructor_074d802cedbbaa1e9784(void * a0) {
    pirates_complete_destructor_074d802cedbbaa1e9784_target(a0);
}

// f-e2656227c1e47dc3c445 — method-forwarder
// TradeUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_e2656227c1e47dc3c445_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_e2656227c1e47dc3c445(void * a0, void * a1, void * a2)
    __asm__("__ZN12TradeUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_e2656227c1e47dc3c445(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_e2656227c1e47dc3c445_target(a0, a1, a2);
}
