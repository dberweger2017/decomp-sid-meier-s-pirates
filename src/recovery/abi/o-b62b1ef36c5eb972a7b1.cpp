// Original compilation group o-b62b1ef36c5eb972a7b1.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-cc4ec1c54f1385e058c6 — method-forwarder
// LostCityUIScene::Update(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Update(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_cc4ec1c54f1385e058c6_target(void *, void *, void *)
    __asm__("__ZN7UIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_cc4ec1c54f1385e058c6(void * a0, void * a1, void * a2)
    __asm__("__ZN15LostCityUIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_cc4ec1c54f1385e058c6(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_cc4ec1c54f1385e058c6_target(a0, a1, a2);
}
