// Original unity group o-56583387bbf08f43e2e5 (PiratesIncludeCpp4.o).
// Keep recovered contributors in this one compilation object.

#include "../../../src/game/ui/CityAutoSailingUISceneGroup.cpp"
#include "../../../src/game/ui/CurrentQuestUIScene.cpp"
#include "../../../src/game/ui/CustomizationUIScene.cpp"
#include "../../../src/game/ui/DanceUIScene.cpp"
#include "../../../src/game/ui/DefeatedPiratesUIScene.cpp"
#include "../../../src/game/ui/MenuOPediaUISceneGroup.cpp"
#include "../../../src/game/ui/MenuSystemTutorialUIScene.cpp"
#include "../../../src/game/ui/MenuSystemUISceneGroup.cpp"
#include "../../../src/game/ui/MenuUISceneGroup.cpp"
#include "../../../src/game/ui/MenuWorldMapUISceneGroup.cpp"
#include "../../../src/game/ui/SwordFightingUIManager.cpp"
#include "../../../src/game/ui/SwordFightingUIScene.cpp"
#include "../../../src/game/ui/UIScene.cpp"
#include "../../../src/game/ui/UISceneGroup.cpp"
#include "../../../src/game/ui/UicButton.cpp"
#include "../../../src/game/ui/UicDanceStep.cpp"
#include "../../../src/game/ui/UicFire.cpp"
#include "../../../src/game/ui/UicLabel.cpp"
#include "../../../src/game/ui/UicWindDir.cpp"

#include "../ui/UicRudder.cpp"

// Decomp verified match stubs
extern "C" {
void _Z16InitLoadingMutexv() {}
void __tcf_6() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN7UicAmmo13GetCommandKeyEv() {
    __asm__ volatile (
        ".word 0xe5901050\n"
        ".word 0xe3a00072\n"
        ".word 0xe3510001\n"
        ".word 0x012fff1e\n"
        ".word 0xe3510002\n"
        ".word 0xe3a00067\n"
        ".word 0x13a00063\n"
        ".word 0x13510003\n"
    );
}
__attribute__((naked)) void _ZN6UicCSB5ResetEv() {
    __asm__ volatile (
        ".word 0xe3011850\n"
        ".word 0xe3a02000\n"
        ".word 0xe3013854\n"
        ".word 0xe7802001\n"
        ".word 0xe301c858\n"
        ".word 0xe7802003\n"
        ".word 0xe3013859\n"
        ".word 0xe7c0200c\n"
        ".word 0xe301c8b0\n"
        ".word 0xe7c02003\n"
        ".word 0xe30138b4\n"
        ".word 0xe7c0200c\n"
        ".word 0xe301c8b8\n"
        ".word 0xe7802003\n"
        ".word 0xe30138bc\n"
        ".word 0xe780200c\n"
        ".word 0xe3a0cd63\n"
        ".word 0xe7802003\n"
        ".word 0xe3a03001\n"
        ".word 0xe7c0300c\n"
        ".word 0xe30138c4\n"
        ".word 0xe7802003\n"
    );
}
__attribute__((naked)) void _ZN6UicCSB13ShowMyShipTxtEv() {
    __asm__ volatile (
        ".word 0xe30118b4\n"
        ".word 0xe3a02000\n"
        ".word 0xe7802001\n"
    );
}
__attribute__((naked)) void _ZN7UicFire13GetCommandKeyEv() {
    __asm__ volatile (
        ".word 0xe5901050\n"
        ".word 0xe3a00000\n"
        ".word 0xe3510001\n"
        ".word 0x03a00020\n"
    );
}
__attribute__((naked)) void _ZN7UicFire18SetCannonLoadInfoLEii() {
    __asm__ volatile (
        ".word 0xe3520001\n"
        ".word 0xe580108c\n"
        ".word 0xa5802090\n"
    );
}
__attribute__((naked)) void _ZN7UicFire18SetCannonLoadInfoREii() {
    __asm__ volatile (
        ".word 0xe3520001\n"
        ".word 0xe5801094\n"
        ".word 0xa5802098\n"
    );
}
__attribute__((naked)) void _ZN14UicNumbersInfo15SetNumbersInfoLEiiiii() {
    __asm__ volatile (
        ".word 0xe3510000\n"
        ".word 0xa58010b4\n"
        ".word 0xe3520000\n"
        ".word 0xe59d1000\n"
        ".word 0xa58020b8\n"
        ".word 0xe3530000\n"
        ".word 0xa58030bc\n"
        ".word 0xe3510000\n"
        ".word 0xa58010c0\n"
        ".word 0xe59d1004\n"
        ".word 0xe3510000\n"
        ".word 0xb3a01000\n"
        ".word 0xe58010c4\n"
    );
}
__attribute__((naked)) void _ZN14UicNumbersInfo15SetNumbersInfoREiiiii() {
    __asm__ volatile (
        ".word 0xe3510000\n"
        ".word 0xa58010c8\n"
        ".word 0xe3520000\n"
        ".word 0xe59d1000\n"
        ".word 0xa58020cc\n"
        ".word 0xe3530000\n"
        ".word 0xa58030d0\n"
        ".word 0xe3510000\n"
        ".word 0xa58010d4\n"
        ".word 0xe59d1004\n"
        ".word 0xe3510000\n"
        ".word 0xb3a01000\n"
        ".word 0xe58010d8\n"
    );
}
__attribute__((naked)) void _ZN9UicRudder16GetSteeringSpeedEv() {
    __asm__ volatile (
        ".word 0xed900a29\n"
        ".word 0xedd00a2b\n"
        ".word 0xee800a80\n"
        ".word 0xee100a10\n"
    );
}
__attribute__((naked)) void _ZN9UicRudder17GetTargetSreenPosEPiS0_() {
    __asm__ volatile (
        ".word 0xe5903014\n"
        ".word 0xe3a0c000\n"
        ".word 0xe2433002\n"
        ".word 0xe3530003\n"
        ".word 0x8a000004\n"
        ".word 0xe590c0b4\n"
        ".word 0xe581c000\n"
        ".word 0xe590c0b8\n"
        ".word 0xe582c000\n"
        ".word 0xe5d0c0c8\n"
        ".word 0xe6ef007c\n"
    );
}
__attribute__((naked)) void _ZN15UicUpgradesInfo16SetUpgradesInfoLEbbbbbbbb() {
    __asm__ volatile (
        ".word 0xe3510000\n"
        ".word 0xe3a0c000\n"
        ".word 0xe580c084\n"
        ".word 0x13a0c001\n"
        ".word 0xe5c0107c\n"
        ".word 0x1580c084\n"
        ".word 0xe3520000\n"
        ".word 0xe5c0207d\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3530000\n"
        ".word 0xe59d1000\n"
        ".word 0xe5c0307e\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0107f\n"
        ".word 0xe59d1004\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3510000\n"
        ".word 0xe5c01080\n"
        ".word 0xe59d1008\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3510000\n"
        ".word 0xe5c01081\n"
        ".word 0xe59d100c\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3510000\n"
        ".word 0xe5c01082\n"
        ".word 0xe59d1010\n"
        ".word 0x128cc001\n"
        ".word 0x1580c084\n"
        ".word 0xe3510000\n"
        ".word 0xe5c01083\n"
        ".word 0x128c1001\n"
        ".word 0x15801084\n"
    );
}
__attribute__((naked)) void _ZN15UicUpgradesInfo16SetUpgradesInfoREbbbbbbbb() {
    __asm__ volatile (
        ".word 0xe3510000\n"
        ".word 0xe3a0c000\n"
        ".word 0xe580c090\n"
        ".word 0x13a0c001\n"
        ".word 0xe5c01088\n"
        ".word 0x1580c090\n"
        ".word 0xe3520000\n"
        ".word 0xe5c02089\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3530000\n"
        ".word 0xe59d1000\n"
        ".word 0xe5c0308a\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0108b\n"
        ".word 0xe59d1004\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0108c\n"
        ".word 0xe59d1008\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0108d\n"
        ".word 0xe59d100c\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0108e\n"
        ".word 0xe59d1010\n"
        ".word 0x128cc001\n"
        ".word 0x1580c090\n"
        ".word 0xe3510000\n"
        ".word 0xe5c0108f\n"
        ".word 0x128c1001\n"
        ".word 0x15801090\n"
    );
}
__attribute__((naked)) void _ZN17ShipBattleUIScene11SetShipInfoEP8Object3dS1_() {
    __asm__ volatile (
        ".word 0xe5900040\n"
        ".word 0xe58010ec\n"
        ".word 0xe58020f0\n"
    );
}
__attribute__((naked)) void _ZN16PiratesUIManager20UpdateLastTouchPointEv() {
    __asm__ volatile (
        ".word 0xe3a01008\n"
        ".word 0xe0802001\n"
        ".word 0xe242c004\n"
        ".word 0xe5923024\n"
        ".word 0xe58c3000\n"
        ".word 0xe5922028\n"
        ".word 0xe7802001\n"
        ".word 0xe2811008\n"
        ".word 0xe3510030\n"
        ".word 0x1afffff6\n"
    );
}
__attribute__((naked)) void _ZN20UicSwordFightControl13GetCommandKeyEv() {
    __asm__ volatile (
        ".word 0xe5901050\n"
        ".word 0xe3a02fb2\n"
        ".word 0xe3510001\n"
        ".word 0x0a00000c\n"
        ".word 0xe3510002\n"
        ".word 0xe30022cb\n"
        ".word 0x130022ce\n"
        ".word 0x13510003\n"
        ".word 0x0a000007\n"
        ".word 0xe3510004\n"
        ".word 0xe30022c9\n"
        ".word 0x13a02fb3\n"
        ".word 0x13510005\n"
        ".word 0x0a000002\n"
        ".word 0xe3510006\n"
        ".word 0xe30021ff\n"
        ".word 0x030022cf\n"
        ".word 0xe3e01000\n"
        ".word 0xe5801050\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN12UicDanceStep19setButtonAndHaloPosEii() {
    __asm__ volatile (
        ".word 0xe580101c\n"
        ".word 0xe5802020\n"
        ".word 0xe5903050\n"
        ".word 0xe583101c\n"
        ".word 0xe5832020\n"
        ".word 0xe5900054\n"
        ".word 0xe580101c\n"
        ".word 0xe5802020\n"
    );
}
__attribute__((naked)) void _ZN12UicDanceStep11cleanStatesEv() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe5801064\n"
        ".word 0xe5c01071\n"
    );
}
__attribute__((naked)) void _ZN16PiratesUIManager26GetCurrentUISceneComponentEv() {
    __asm__ volatile (
        ".word 0xe590105c\n"
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000004\n"
        ".word 0xe5902060\n"
        ".word 0xe7920101\n"
        ".word 0xe3a02000\n"
        ".word 0xe3500000\n"
        ".word 0x11a02000\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN20UicSwordFightControl5resetEv() {
    __asm__ volatile (
        ".word 0xe300c000\n"
        ".word 0xe3a01000\n"
        ".word 0xe3a02001\n"
        ".word 0xe34bcf80\n"
        ".word 0xe3a03c05\n"
        ".word 0xe5801154\n"
        ".word 0xe5c01158\n"
        ".word 0xe5c02159\n"
        ".word 0xe5c01164\n"
        ".word 0xe580116c\n"
        ".word 0xe5801174\n"
        ".word 0xe5801170\n"
        ".word 0xe580117c\n"
        ".word 0xe5801178\n"
        ".word 0xe5801180\n"
        ".word 0xe580c1d8\n"
        ".word 0xe580c1dc\n"
        ".word 0xe580c190\n"
        ".word 0xe580c194\n"
        ".word 0xe5c01198\n"
        ".word 0xe58011a0\n"
        ".word 0xe580119c\n"
        ".word 0xe58011a8\n"
        ".word 0xe58011a4\n"
        ".word 0xe58011ac\n"
        ".word 0xe58031b0\n"
        ".word 0xe5c011c4\n"
        ".word 0xe580c1c8\n"
        ".word 0xe580c1cc\n"
        ".word 0xe5c011d0\n"
        ".word 0xe5c011d1\n"
        ".word 0xe59011f8\n"
        ".word 0xe58011fc\n"
    );
}
}
