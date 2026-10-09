// Original compilation group o-11f52d80c803677093fa.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-2b11537c8a3b32d11267 — method-forwarder
// LostRelativeUIScene::Update(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Update(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_2b11537c8a3b32d11267_target(void *, void *, void *)
    __asm__("__ZN7UIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_2b11537c8a3b32d11267(void * a0, void * a1, void * a2)
    __asm__("__ZN19LostRelativeUIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_2b11537c8a3b32d11267(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_2b11537c8a3b32d11267_target(a0, a1, a2);
}
