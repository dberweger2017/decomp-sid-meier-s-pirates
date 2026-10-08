// Original compilation group o-cbb1300000f57e037386.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-2fc275d3de7cc083be3d — complete-constructor
// SaveLoadUIScene::SaveLoadUIScene(CPVRTString const&, UISceneComponent*, SLUISceneType)
// Calls: SaveLoadUIScene::SaveLoadUIScene(CPVRTString const&, UISceneComponent*, SLUISceneType)
extern "C" void pirates_complete_constructor_2fc275d3de7cc083be3d_target(void *, const void *, void *, int)
    __asm__("__ZN15SaveLoadUISceneC2ERK11CPVRTStringP16UISceneComponent13SLUISceneType");
extern "C" void pirates_complete_constructor_2fc275d3de7cc083be3d(void * a0, const void * a1, void * a2, int a3)
    __asm__("__ZN15SaveLoadUISceneC1ERK11CPVRTStringP16UISceneComponent13SLUISceneType");
extern "C" void pirates_complete_constructor_2fc275d3de7cc083be3d(void * a0, const void * a1, void * a2, int a3) {
    pirates_complete_constructor_2fc275d3de7cc083be3d_target(a0, a1, a2, a3);
}

// f-86f0f3e058a4392b7cb5 — complete-destructor
// SaveLoadUIScene::~SaveLoadUIScene()
// Calls: SaveLoadUIScene::~SaveLoadUIScene()
extern "C" void pirates_complete_destructor_86f0f3e058a4392b7cb5_target(void *)
    __asm__("__ZN15SaveLoadUISceneD2Ev");
extern "C" void pirates_complete_destructor_86f0f3e058a4392b7cb5(void * a0)
    __asm__("__ZN15SaveLoadUISceneD1Ev");
extern "C" void pirates_complete_destructor_86f0f3e058a4392b7cb5(void * a0) {
    pirates_complete_destructor_86f0f3e058a4392b7cb5_target(a0);
}
