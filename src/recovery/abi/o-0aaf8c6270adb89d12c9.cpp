// Original compilation group o-0aaf8c6270adb89d12c9.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-017fbd46b57da1d0f83f — complete-constructor
// Shipwright::Shipwright(int, int)
// Calls: Shipwright::Shipwright(int, int)
extern "C" void pirates_complete_constructor_017fbd46b57da1d0f83f_target(void *, int, int)
    __asm__("__ZN10ShipwrightC2Eii");
extern "C" void pirates_complete_constructor_017fbd46b57da1d0f83f(void * a0, int a1, int a2)
    __asm__("__ZN10ShipwrightC1Eii");
extern "C" void pirates_complete_constructor_017fbd46b57da1d0f83f(void * a0, int a1, int a2) {
    pirates_complete_constructor_017fbd46b57da1d0f83f_target(a0, a1, a2);
}

// f-05a159c450b97b923274 — complete-destructor
// AudioQueue::~AudioQueue()
// Calls: AudioQueue::~AudioQueue()
extern "C" void pirates_complete_destructor_05a159c450b97b923274_target(void *)
    __asm__("__ZN10AudioQueueD2Ev");
extern "C" void pirates_complete_destructor_05a159c450b97b923274(void * a0)
    __asm__("__ZN10AudioQueueD1Ev");
extern "C" void pirates_complete_destructor_05a159c450b97b923274(void * a0) {
    pirates_complete_destructor_05a159c450b97b923274_target(a0);
}

// f-0c9240d445cc55045918 — complete-constructor
// BattleGrid::BattleGrid()
// Calls: BattleGrid::BattleGrid()
extern "C" void pirates_complete_constructor_0c9240d445cc55045918_target(void *)
    __asm__("__ZN10BattleGridC2Ev");
extern "C" void pirates_complete_constructor_0c9240d445cc55045918(void * a0)
    __asm__("__ZN10BattleGridC1Ev");
extern "C" void pirates_complete_constructor_0c9240d445cc55045918(void * a0) {
    pirates_complete_constructor_0c9240d445cc55045918_target(a0);
}

// f-0cf5f9f7713a7e80182f — function-forwarder
// CCDist(int, int)
// Calls: CityDist(int, int)
extern "C" int pirates_function_forwarder_0cf5f9f7713a7e80182f_target(int, int)
    __asm__("__Z8CityDistii");
extern "C" int pirates_function_forwarder_0cf5f9f7713a7e80182f(int a0, int a1)
    __asm__("__Z6CCDistii");
extern "C" int pirates_function_forwarder_0cf5f9f7713a7e80182f(int a0, int a1) {
    return pirates_function_forwarder_0cf5f9f7713a7e80182f_target(a0, a1);
}

// f-10e9b2d096311c5db3cd — complete-destructor
// NiTPointerMap<NiNode*, Object3d*>::~NiTPointerMap()
// Calls: NiTPointerMap<NiNode*, Object3d*>::~NiTPointerMap()
extern "C" void pirates_complete_destructor_10e9b2d096311c5db3cd_target(void *)
    __asm__("__ZN13NiTPointerMapIP6NiNodeP8Object3dED2Ev");
extern "C" void pirates_complete_destructor_10e9b2d096311c5db3cd(void * a0)
    __asm__("__ZN13NiTPointerMapIP6NiNodeP8Object3dED1Ev");
extern "C" void pirates_complete_destructor_10e9b2d096311c5db3cd(void * a0) {
    pirates_complete_destructor_10e9b2d096311c5db3cd_target(a0);
}

// f-1717fabd668c9e68376f — complete-destructor
// NiTPointerList<WorldTerraformTemporary*>::~NiTPointerList()
// Calls: NiTPointerList<WorldTerraformTemporary*>::~NiTPointerList()
extern "C" void pirates_complete_destructor_1717fabd668c9e68376f_target(void *)
    __asm__("__ZN14NiTPointerListIP23WorldTerraformTemporaryED2Ev");
extern "C" void pirates_complete_destructor_1717fabd668c9e68376f(void * a0)
    __asm__("__ZN14NiTPointerListIP23WorldTerraformTemporaryED1Ev");
extern "C" void pirates_complete_destructor_1717fabd668c9e68376f(void * a0) {
    pirates_complete_destructor_1717fabd668c9e68376f_target(a0);
}

// f-178170665e112b92ecc0 — complete-destructor
// gullz::~gullz()
// Calls: gullz::~gullz()
extern "C" void pirates_complete_destructor_178170665e112b92ecc0_target(void *)
    __asm__("__ZN5gullzD2Ev");
extern "C" void pirates_complete_destructor_178170665e112b92ecc0(void * a0)
    __asm__("__ZN5gullzD1Ev");
extern "C" void pirates_complete_destructor_178170665e112b92ecc0(void * a0) {
    pirates_complete_destructor_178170665e112b92ecc0_target(a0);
}

// f-1c013762039ef83f0b2d — complete-destructor
// DanceReflector::~DanceReflector()
// Calls: DanceReflector::~DanceReflector()
extern "C" void pirates_complete_destructor_1c013762039ef83f0b2d_target(void *)
    __asm__("__ZN14DanceReflectorD2Ev");
extern "C" void pirates_complete_destructor_1c013762039ef83f0b2d(void * a0)
    __asm__("__ZN14DanceReflectorD1Ev");
extern "C" void pirates_complete_destructor_1c013762039ef83f0b2d(void * a0) {
    pirates_complete_destructor_1c013762039ef83f0b2d_target(a0);
}

// f-246beee04fff1fe31475 — destruction-callback
// __tcf_1
// Calls: agentz_manager::Clean()
extern "C" void pirates_destruction_callback_246beee04fff1fe31475_target(void)
    __asm__("__ZN14agentz_manager5CleanEv");
static void pirates_destruction_callback_246beee04fff1fe31475(void * a0)
    __asm__("___tcf_1");
static void pirates_destruction_callback_246beee04fff1fe31475(void * a0) {
    pirates_destruction_callback_246beee04fff1fe31475_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_destruction_callback_246beee04fff1fe31475_source_reference)(void *) = pirates_destruction_callback_246beee04fff1fe31475;

// f-25d108778847f994b7db — complete-destructor
// UicAnimator::~UicAnimator()
// Calls: UicAnimator::~UicAnimator()
extern "C" void pirates_complete_destructor_25d108778847f994b7db_target(void *)
    __asm__("__ZN11UicAnimatorD2Ev");
extern "C" void pirates_complete_destructor_25d108778847f994b7db(void * a0)
    __asm__("__ZN11UicAnimatorD1Ev");
extern "C" void pirates_complete_destructor_25d108778847f994b7db(void * a0) {
    pirates_complete_destructor_25d108778847f994b7db_target(a0);
}

// f-2cb5550b2ccf419e8baf — complete-destructor
// NiTMapBase<DFALL<int>, FStringA*, int>::~NiTMapBase()
// Calls: NiTMapBase<DFALL<int>, FStringA*, int>::~NiTMapBase()
extern "C" void pirates_complete_destructor_2cb5550b2ccf419e8baf_target(void *)
    __asm__("__ZN10NiTMapBaseI5DFALLIiEP8FStringAiED2Ev");
extern "C" void pirates_complete_destructor_2cb5550b2ccf419e8baf(void * a0)
    __asm__("__ZN10NiTMapBaseI5DFALLIiEP8FStringAiED1Ev");
extern "C" void pirates_complete_destructor_2cb5550b2ccf419e8baf(void * a0) {
    pirates_complete_destructor_2cb5550b2ccf419e8baf_target(a0);
}

// f-3aa95dcf8f8b08794bd7 — complete-destructor
// ISE::MyApp::~MyApp()
// Calls: ISE::MyApp::~MyApp()
extern "C" void pirates_complete_destructor_3aa95dcf8f8b08794bd7_target(void *)
    __asm__("__ZN3ISE5MyAppD2Ev");
extern "C" void pirates_complete_destructor_3aa95dcf8f8b08794bd7(void * a0)
    __asm__("__ZN3ISE5MyAppD1Ev");
extern "C" void pirates_complete_destructor_3aa95dcf8f8b08794bd7(void * a0) {
    pirates_complete_destructor_3aa95dcf8f8b08794bd7_target(a0);
}

// f-3d457aab7bbc33edddc9 — complete-destructor
// UicDots::~UicDots()
// Calls: UicDots::~UicDots()
extern "C" void pirates_complete_destructor_3d457aab7bbc33edddc9_target(void *)
    __asm__("__ZN7UicDotsD2Ev");
extern "C" void pirates_complete_destructor_3d457aab7bbc33edddc9(void * a0)
    __asm__("__ZN7UicDotsD1Ev");
extern "C" void pirates_complete_destructor_3d457aab7bbc33edddc9(void * a0) {
    pirates_complete_destructor_3d457aab7bbc33edddc9_target(a0);
}

// f-477304d66bd06114d1a5 — method-forwarder
// NiTMap<FStringA*, int>::NewItem()
// Calls: DFALL<int>::Allocate()
extern "C" void * pirates_method_forwarder_477304d66bd06114d1a5_target(void)
    __asm__("__ZN5DFALLIiE8AllocateEv");
extern "C" void * pirates_method_forwarder_477304d66bd06114d1a5(void * a0)
    __asm__("__ZN6NiTMapIP8FStringAiE7NewItemEv");
extern "C" void * pirates_method_forwarder_477304d66bd06114d1a5(void * a0) {
    return pirates_method_forwarder_477304d66bd06114d1a5_target();
}

// f-4b13347d448010c37039 — method-forwarder
// RegistrationUIScene::Render(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Render(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_4b13347d448010c37039_target(void *, void *, void *)
    __asm__("__ZN7UIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_4b13347d448010c37039(void * a0, void * a1, void * a2)
    __asm__("__ZN19RegistrationUIScene6RenderEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_4b13347d448010c37039(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_4b13347d448010c37039_target(a0, a1, a2);
}

// f-4c1ac6090cca33f5b26f — complete-constructor
// MenuScreen::MenuScreen()
// Calls: MenuScreen::MenuScreen()
extern "C" void pirates_complete_constructor_4c1ac6090cca33f5b26f_target(void *)
    __asm__("__ZN10MenuScreenC2Ev");
extern "C" void pirates_complete_constructor_4c1ac6090cca33f5b26f(void * a0)
    __asm__("__ZN10MenuScreenC1Ev");
extern "C" void pirates_complete_constructor_4c1ac6090cca33f5b26f(void * a0) {
    pirates_complete_constructor_4c1ac6090cca33f5b26f_target(a0);
}

// f-4fe1329e6c81895ed69c — complete-constructor
// TopTenPiratesUIScene::TopTenPiratesUIScene(CPVRTString const&, UISceneComponent*)
// Calls: TopTenPiratesUIScene::TopTenPiratesUIScene(CPVRTString const&, UISceneComponent*)
extern "C" void pirates_complete_constructor_4fe1329e6c81895ed69c_target(void *, const void *, void *)
    __asm__("__ZN20TopTenPiratesUISceneC2ERK11CPVRTStringP16UISceneComponent");
extern "C" void pirates_complete_constructor_4fe1329e6c81895ed69c(void * a0, const void * a1, void * a2)
    __asm__("__ZN20TopTenPiratesUISceneC1ERK11CPVRTStringP16UISceneComponent");
extern "C" void pirates_complete_constructor_4fe1329e6c81895ed69c(void * a0, const void * a1, void * a2) {
    pirates_complete_constructor_4fe1329e6c81895ed69c_target(a0, a1, a2);
}

// f-51a4b29dc0fdd32e110f — complete-destructor
// FStringMap<int>::~FStringMap()
// Calls: FStringMap<int>::~FStringMap()
extern "C" void pirates_complete_destructor_51a4b29dc0fdd32e110f_target(void *)
    __asm__("__ZN10FStringMapIiED2Ev");
extern "C" void pirates_complete_destructor_51a4b29dc0fdd32e110f(void * a0)
    __asm__("__ZN10FStringMapIiED1Ev");
extern "C" void pirates_complete_destructor_51a4b29dc0fdd32e110f(void * a0) {
    pirates_complete_destructor_51a4b29dc0fdd32e110f_target(a0);
}

// f-5581c01b694f6bd5d07a — complete-destructor
// FleetStatusUIScene::~FleetStatusUIScene()
// Calls: FleetStatusUIScene::~FleetStatusUIScene()
extern "C" void pirates_complete_destructor_5581c01b694f6bd5d07a_target(void *)
    __asm__("__ZN18FleetStatusUISceneD2Ev");
extern "C" void pirates_complete_destructor_5581c01b694f6bd5d07a(void * a0)
    __asm__("__ZN18FleetStatusUISceneD1Ev");
extern "C" void pirates_complete_destructor_5581c01b694f6bd5d07a(void * a0) {
    pirates_complete_destructor_5581c01b694f6bd5d07a_target(a0);
}

// f-5ddd41cfdbbbca96d795 — complete-constructor
// Wake::Wake(ISE::ISEPointer<ISE::ISEEntity>, float)
// Calls: Wake::Wake(ISE::ISEPointer<ISE::ISEEntity>, float)
extern "C" void pirates_complete_constructor_5ddd41cfdbbbca96d795_target(void *, void *, float)
    __asm__("__ZN4WakeC2EN3ISE10ISEPointerINS0_9ISEEntityEEEf");
extern "C" void pirates_complete_constructor_5ddd41cfdbbbca96d795(void * a0, void * a1, float a2)
    __asm__("__ZN4WakeC1EN3ISE10ISEPointerINS0_9ISEEntityEEEf");
extern "C" void pirates_complete_constructor_5ddd41cfdbbbca96d795(void * a0, void * a1, float a2) {
    pirates_complete_constructor_5ddd41cfdbbbca96d795_target(a0, a1, a2);
}

// f-681d2124e4d491617774 — complete-destructor
// Shipwright::~Shipwright()
// Calls: Shipwright::~Shipwright()
extern "C" void pirates_complete_destructor_681d2124e4d491617774_target(void *)
    __asm__("__ZN10ShipwrightD2Ev");
extern "C" void pirates_complete_destructor_681d2124e4d491617774(void * a0)
    __asm__("__ZN10ShipwrightD1Ev");
extern "C" void pirates_complete_destructor_681d2124e4d491617774(void * a0) {
    pirates_complete_destructor_681d2124e4d491617774_target(a0);
}

// f-7c47cdb67941fae157e4 — complete-destructor
// StoreAndItemUIScene::~StoreAndItemUIScene()
// Calls: StoreAndItemUIScene::~StoreAndItemUIScene()
extern "C" void pirates_complete_destructor_7c47cdb67941fae157e4_target(void *)
    __asm__("__ZN19StoreAndItemUISceneD2Ev");
extern "C" void pirates_complete_destructor_7c47cdb67941fae157e4(void * a0)
    __asm__("__ZN19StoreAndItemUISceneD1Ev");
extern "C" void pirates_complete_destructor_7c47cdb67941fae157e4(void * a0) {
    pirates_complete_destructor_7c47cdb67941fae157e4_target(a0);
}

// f-857402110ac29947ab62 — complete-destructor
// NiTPointerMap<unsigned char, FStringA*>::~NiTPointerMap()
// Calls: NiTPointerMap<unsigned char, FStringA*>::~NiTPointerMap()
extern "C" void pirates_complete_destructor_857402110ac29947ab62_target(void *)
    __asm__("__ZN13NiTPointerMapIhP8FStringAED2Ev");
extern "C" void pirates_complete_destructor_857402110ac29947ab62(void * a0)
    __asm__("__ZN13NiTPointerMapIhP8FStringAED1Ev");
extern "C" void pirates_complete_destructor_857402110ac29947ab62(void * a0) {
    pirates_complete_destructor_857402110ac29947ab62_target(a0);
}

// f-92c036cc9a71419d04bc — complete-destructor
// World::~World()
// Calls: World::~World()
extern "C" void pirates_complete_destructor_92c036cc9a71419d04bc_target(void *)
    __asm__("__ZN5WorldD2Ev");
extern "C" void pirates_complete_destructor_92c036cc9a71419d04bc(void * a0)
    __asm__("__ZN5WorldD1Ev");
extern "C" void pirates_complete_destructor_92c036cc9a71419d04bc(void * a0) {
    pirates_complete_destructor_92c036cc9a71419d04bc_target(a0);
}

// f-a25b3ec52d90e41e3242 — complete-constructor
// DanceReflector::DanceReflector()
// Calls: DanceReflector::DanceReflector()
extern "C" void pirates_complete_constructor_a25b3ec52d90e41e3242_target(void *)
    __asm__("__ZN14DanceReflectorC2Ev");
extern "C" void pirates_complete_constructor_a25b3ec52d90e41e3242(void * a0)
    __asm__("__ZN14DanceReflectorC1Ev");
extern "C" void pirates_complete_constructor_a25b3ec52d90e41e3242(void * a0) {
    pirates_complete_constructor_a25b3ec52d90e41e3242_target(a0);
}

// f-a90018bcd0fc324b1f1c — complete-destructor
// dolphinz::~dolphinz()
// Calls: dolphinz::~dolphinz()
extern "C" void pirates_complete_destructor_a90018bcd0fc324b1f1c_target(void *)
    __asm__("__ZN8dolphinzD2Ev");
extern "C" void pirates_complete_destructor_a90018bcd0fc324b1f1c(void * a0)
    __asm__("__ZN8dolphinzD1Ev");
extern "C" void pirates_complete_destructor_a90018bcd0fc324b1f1c(void * a0) {
    pirates_complete_destructor_a90018bcd0fc324b1f1c_target(a0);
}

// f-b73a8617f6406029482b — method-forwarder
// NiTPointerListBase<DFALL<EFFECTINFO*>, EFFECTINFO*>::NewItem()
// Calls: DFALL<EFFECTINFO*>::Allocate()
extern "C" void * pirates_method_forwarder_b73a8617f6406029482b_target(void)
    __asm__("__ZN5DFALLIP10EFFECTINFOE8AllocateEv");
extern "C" void * pirates_method_forwarder_b73a8617f6406029482b(void * a0)
    __asm__("__ZN18NiTPointerListBaseI5DFALLIP10EFFECTINFOES2_E7NewItemEv");
extern "C" void * pirates_method_forwarder_b73a8617f6406029482b(void * a0) {
    return pirates_method_forwarder_b73a8617f6406029482b_target();
}

// f-b8c8a8d7bdefd3f6610c — complete-destructor
// RegistrationUIScene::~RegistrationUIScene()
// Calls: RegistrationUIScene::~RegistrationUIScene()
extern "C" void pirates_complete_destructor_b8c8a8d7bdefd3f6610c_target(void *)
    __asm__("__ZN19RegistrationUISceneD2Ev");
extern "C" void pirates_complete_destructor_b8c8a8d7bdefd3f6610c(void * a0)
    __asm__("__ZN19RegistrationUISceneD1Ev");
extern "C" void pirates_complete_destructor_b8c8a8d7bdefd3f6610c(void * a0) {
    pirates_complete_destructor_b8c8a8d7bdefd3f6610c_target(a0);
}

// f-babaea7c41dfa43d6740 — complete-destructor
// NiTMap<FStringA*, int>::~NiTMap()
// Calls: NiTMap<FStringA*, int>::~NiTMap()
extern "C" void pirates_complete_destructor_babaea7c41dfa43d6740_target(void *)
    __asm__("__ZN6NiTMapIP8FStringAiED2Ev");
extern "C" void pirates_complete_destructor_babaea7c41dfa43d6740(void * a0)
    __asm__("__ZN6NiTMapIP8FStringAiED1Ev");
extern "C" void pirates_complete_destructor_babaea7c41dfa43d6740(void * a0) {
    pirates_complete_destructor_babaea7c41dfa43d6740_target(a0);
}

// f-cd92450d8576f602e1fe — complete-destructor
// Wake::~Wake()
// Calls: Wake::~Wake()
extern "C" void pirates_complete_destructor_cd92450d8576f602e1fe_target(void *)
    __asm__("__ZN4WakeD2Ev");
extern "C" void pirates_complete_destructor_cd92450d8576f602e1fe(void * a0)
    __asm__("__ZN4WakeD1Ev");
extern "C" void pirates_complete_destructor_cd92450d8576f602e1fe(void * a0) {
    pirates_complete_destructor_cd92450d8576f602e1fe_target(a0);
}

// f-d29576bba16946a09915 — complete-destructor
// BattleGrid::~BattleGrid()
// Calls: BattleGrid::~BattleGrid()
extern "C" void pirates_complete_destructor_d29576bba16946a09915_target(void *)
    __asm__("__ZN10BattleGridD2Ev");
extern "C" void pirates_complete_destructor_d29576bba16946a09915(void * a0)
    __asm__("__ZN10BattleGridD1Ev");
extern "C" void pirates_complete_destructor_d29576bba16946a09915(void * a0) {
    pirates_complete_destructor_d29576bba16946a09915_target(a0);
}

// f-d2aa7dca780e78ca91ec — complete-constructor
// FleetStatusUIScene::FleetStatusUIScene(CPVRTString const&, UISceneComponent*)
// Calls: FleetStatusUIScene::FleetStatusUIScene(CPVRTString const&, UISceneComponent*)
extern "C" void pirates_complete_constructor_d2aa7dca780e78ca91ec_target(void *, const void *, void *)
    __asm__("__ZN18FleetStatusUISceneC2ERK11CPVRTStringP16UISceneComponent");
extern "C" void pirates_complete_constructor_d2aa7dca780e78ca91ec(void * a0, const void * a1, void * a2)
    __asm__("__ZN18FleetStatusUISceneC1ERK11CPVRTStringP16UISceneComponent");
extern "C" void pirates_complete_constructor_d2aa7dca780e78ca91ec(void * a0, const void * a1, void * a2) {
    pirates_complete_constructor_d2aa7dca780e78ca91ec_target(a0, a1, a2);
}

// f-d308ec448c7971ac02b6 — complete-destructor
// FlagSailModUIScene::~FlagSailModUIScene()
// Calls: FlagSailModUIScene::~FlagSailModUIScene()
extern "C" void pirates_complete_destructor_d308ec448c7971ac02b6_target(void *)
    __asm__("__ZN18FlagSailModUISceneD2Ev");
extern "C" void pirates_complete_destructor_d308ec448c7971ac02b6(void * a0)
    __asm__("__ZN18FlagSailModUISceneD1Ev");
extern "C" void pirates_complete_destructor_d308ec448c7971ac02b6(void * a0) {
    pirates_complete_destructor_d308ec448c7971ac02b6_target(a0);
}

// f-d341a1662ad2e91add9e — complete-destructor
// NiTList<EFFECTINFO*>::~NiTList()
// Calls: NiTList<EFFECTINFO*>::~NiTList()
extern "C" void pirates_complete_destructor_d341a1662ad2e91add9e_target(void *)
    __asm__("__ZN7NiTListIP10EFFECTINFOED2Ev");
extern "C" void pirates_complete_destructor_d341a1662ad2e91add9e(void * a0)
    __asm__("__ZN7NiTListIP10EFFECTINFOED1Ev");
extern "C" void pirates_complete_destructor_d341a1662ad2e91add9e(void * a0) {
    pirates_complete_destructor_d341a1662ad2e91add9e_target(a0);
}

// f-d5be56d689683bed7d0b — complete-destructor
// NiTPointerList<WorldTerraformPermanent*>::~NiTPointerList()
// Calls: NiTPointerList<WorldTerraformPermanent*>::~NiTPointerList()
extern "C" void pirates_complete_destructor_d5be56d689683bed7d0b_target(void *)
    __asm__("__ZN14NiTPointerListIP23WorldTerraformPermanentED2Ev");
extern "C" void pirates_complete_destructor_d5be56d689683bed7d0b(void * a0)
    __asm__("__ZN14NiTPointerListIP23WorldTerraformPermanentED1Ev");
extern "C" void pirates_complete_destructor_d5be56d689683bed7d0b(void * a0) {
    pirates_complete_destructor_d5be56d689683bed7d0b_target(a0);
}

// f-e1c6ffa62028df30ad37 — complete-destructor
// FStack<Opedia::FPediaToken, 10ul>::~FStack()
// Calls: FStack<Opedia::FPediaToken, 10ul>::~FStack()
extern "C" void pirates_complete_destructor_e1c6ffa62028df30ad37_target(void *)
    __asm__("__ZN6FStackIN6Opedia11FPediaTokenELm10EED2Ev");
extern "C" void pirates_complete_destructor_e1c6ffa62028df30ad37(void * a0)
    __asm__("__ZN6FStackIN6Opedia11FPediaTokenELm10EED1Ev");
extern "C" void pirates_complete_destructor_e1c6ffa62028df30ad37(void * a0) {
    pirates_complete_destructor_e1c6ffa62028df30ad37_target(a0);
}

// f-e74e59b613bba32f2ccb — complete-destructor
// TopTenPiratesUIScene::~TopTenPiratesUIScene()
// Calls: TopTenPiratesUIScene::~TopTenPiratesUIScene()
extern "C" void pirates_complete_destructor_e74e59b613bba32f2ccb_target(void *)
    __asm__("__ZN20TopTenPiratesUISceneD2Ev");
extern "C" void pirates_complete_destructor_e74e59b613bba32f2ccb(void * a0)
    __asm__("__ZN20TopTenPiratesUISceneD1Ev");
extern "C" void pirates_complete_destructor_e74e59b613bba32f2ccb(void * a0) {
    pirates_complete_destructor_e74e59b613bba32f2ccb_target(a0);
}

// f-e94818195a88450643d6 — complete-destructor
// NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::~NiTMapBase()
// Calls: NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::~NiTMapBase()
extern "C" void pirates_complete_destructor_e94818195a88450643d6_target(void *)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEP6NiNodeP8Object3dED2Ev");
extern "C" void pirates_complete_destructor_e94818195a88450643d6(void * a0)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEP6NiNodeP8Object3dED1Ev");
extern "C" void pirates_complete_destructor_e94818195a88450643d6(void * a0) {
    pirates_complete_destructor_e94818195a88450643d6_target(a0);
}

// f-ee61d3f645061fe6f205 — complete-destructor
// NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::~NiTMapBase()
// Calls: NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::~NiTMapBase()
extern "C" void pirates_complete_destructor_ee61d3f645061fe6f205_target(void *)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEhP8FStringAED2Ev");
extern "C" void pirates_complete_destructor_ee61d3f645061fe6f205(void * a0)
    __asm__("__ZN10NiTMapBaseI19NiTPointerAllocatorIjEhP8FStringAED1Ev");
extern "C" void pirates_complete_destructor_ee61d3f645061fe6f205(void * a0) {
    pirates_complete_destructor_ee61d3f645061fe6f205_target(a0);
}

// f-fd769b5e5665f977199c — method-forwarder
// CityInfoUIScene::Update(PVRTVec2*, PVRTVec2*)
// Calls: UIScene::Update(PVRTVec2*, PVRTVec2*)
extern "C" void pirates_method_forwarder_fd769b5e5665f977199c_target(void *, void *, void *)
    __asm__("__ZN7UIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_fd769b5e5665f977199c(void * a0, void * a1, void * a2)
    __asm__("__ZN15CityInfoUIScene6UpdateEP8PVRTVec2S1_");
extern "C" void pirates_method_forwarder_fd769b5e5665f977199c(void * a0, void * a1, void * a2) {
    pirates_method_forwarder_fd769b5e5665f977199c_target(a0, a1, a2);
}

// f-fd90f818d468d9e44764 — complete-destructor
// UicComboButton::~UicComboButton()
// Calls: UicComboButton::~UicComboButton()
extern "C" void pirates_complete_destructor_fd90f818d468d9e44764_target(void *)
    __asm__("__ZN14UicComboButtonD2Ev");
extern "C" void pirates_complete_destructor_fd90f818d468d9e44764(void * a0)
    __asm__("__ZN14UicComboButtonD1Ev");
extern "C" void pirates_complete_destructor_fd90f818d468d9e44764(void * a0) {
    pirates_complete_destructor_fd90f818d468d9e44764_target(a0);
}
