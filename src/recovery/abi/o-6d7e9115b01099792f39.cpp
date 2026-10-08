// Original compilation group o-6d7e9115b01099792f39.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-24c6449732a4cae02001 — method-forwarder
// TreasureUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_24c6449732a4cae02001_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_24c6449732a4cae02001(void * a0, void * a1, void * a2)
    __asm__("__ZN15TreasureUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_24c6449732a4cae02001(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_24c6449732a4cae02001_target(a0, a1, a2);
}

// f-697a35008785c1bd4780 — complete-destructor
// TreasureUIScene::~TreasureUIScene()
// Calls: TreasureUIScene::~TreasureUIScene()
extern "C" void pirates_complete_destructor_697a35008785c1bd4780_target(void *)
    __asm__("__ZN15TreasureUISceneD2Ev");
extern "C" void pirates_complete_destructor_697a35008785c1bd4780(void * a0)
    __asm__("__ZN15TreasureUISceneD1Ev");
extern "C" void pirates_complete_destructor_697a35008785c1bd4780(void * a0) {
    pirates_complete_destructor_697a35008785c1bd4780_target(a0);
}

// f-a0ae5a7c6d7ed3ac2c93 — method-forwarder
// TreasureUIScene::Update(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Update(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_a0ae5a7c6d7ed3ac2c93_target(void *, void *, void *)
    __asm__("__ZN7UIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a0ae5a7c6d7ed3ac2c93(void * a0, void * a1, void * a2)
    __asm__("__ZN15TreasureUIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_a0ae5a7c6d7ed3ac2c93(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_a0ae5a7c6d7ed3ac2c93_target(a0, a1, a2);
}
