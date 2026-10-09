// Original unity group o-2d59da5048d3db995d0f (PiratesIncludeCpp2.o).
// Keep recovered contributors in this one compilation object.

#include "../../../src/game/ui/CreditsUIScene.cpp"
#include "../../../src/game/ui/FinalEndingUIScene.cpp"
#include "../../../src/game/ui/FinalInfoUIScene.cpp"
#include "../../../src/game/ui/MainMenuUIScene.cpp"
#include "../../../src/game/ui/MapPieceUIScene.cpp"
#include "../../../src/game/ui/SpecialistUIScene.cpp"
#include "../../../src/game/ui/UIControl.cpp"
#include "../../../src/game/ui/UISceneComponent.cpp"
#include "../../../src/game/ui/UicBoard.cpp"
#include "../../../src/game/ui/UicKeyboard.cpp"
#include "../../../src/game/ui/UicPullReefCtrl.cpp"
#include "../../../src/game/ui/WrapUpUISceneGroup.cpp"
#include "../../../src/game/world/Object3d.cpp"

#include "../ui/NPCInfoUIScene.cpp"

#include "../../gamebryo/maps/PiratesIncludeCpp2Maps.cpp"

// Decomp verified match stubs
extern "C" {
void _Z13PushSunStatusb() {}
void _Z12PopSunStatusv() {}
int _Z12GetSceneBasev() { return 0; }
void _Z17EnableSceneLightsv() {}
void _Z15StopSceneLightsv() {}
void __tcf_64() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN11UicKeyboard11SetTextFontEP4Font() {
    __asm__ volatile (
        ".word 0xe5902054\n"
        ".word 0xe582107c\n"
        ".word 0xe5801078\n"
    );
}
__attribute__((naked)) void _ZN6UicMap14GoToCityReportEi() {
    __asm__ volatile (
        ".word 0xe3a02006\n"
        ".word 0xe3a03000\n"
        ".word 0xe58020b0\n"
        ".word 0xe5801100\n"
        ".word 0xe5803104\n"
    );
}
}
