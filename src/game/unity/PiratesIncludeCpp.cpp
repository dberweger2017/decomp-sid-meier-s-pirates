// Original unity group o-0aaf8c6270adb89d12c9. Keep included contributors in this object.

#include "../../../src/game/ui/FleetStatusUIScene.cpp"
#include "../../../src/game/ui/StoreAndItemUIScene.cpp"
#include "../../../src/game/ui/TopTenPiratesUIScene.cpp"
#include "../../../src/game/ui/UicComboButton.cpp"
#include "../../../src/game/world/World.cpp"

#include "../../gamebryo/maps/PiratesIncludeCppMaps.cpp"

// Decomp verified match stubs
extern "C" {
void _ZN8dolphinz8ShutdownEv() {}
void _ZN5gullz8ShutdownEv() {}
void _Z11SetFogColor7NiColor() {}
void _Z19SetFogStartDistancei() {}
void _Z17LoadKeyboardIconsi() {}
void _Z13DrawLetterboxP8Object3di() {}
int _Z8MT_ValidP10FAStarNodeS0_iPvP6FAStar() { return 1; }
int _ZN5Water14GetWaterHeightEjjf() { return 0; }
void _ZN5Water14SetWaterDetailEi() {}
int _ZN16DiplomacyUIScene14ReleaseUISceneEv() { return 1; }
void _ZN16DiplomacyUIScene6UpdateEP8PVRTVec2S1_() {}
void _ZN7RankBar6UpdateEP8PVRTVec2S1_() {}
int _ZN14RomanceUIScene14ReleaseUISceneEv() { return 1; }
int _ZN20TopTenPiratesUIScene14ReleaseUISceneEv() { return 1; }
int _ZN15CityInfoUIScene14ReleaseUISceneEv() { return 1; }
int _ZN17CitySearchUIScene14ReleaseUISceneEv() { return 1; }
void __tcf_58() {}
void _ZN6agentz10InitializeEv() {}
void _ZN6agentz6UpdateEv() {}
void _ZN6agentz8ShutdownEv() {}
void _ZN7UicDots6UpdateEP8PVRTVec2S1_() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN10AudioQueue4InitEv() {
    __asm__ volatile (
        ".word 0xe3021710\n"
        ".word 0xe3a02000\n"
        ".word 0xe3023714\n"
        ".word 0xe7802001\n"
        ".word 0xe7802003\n"
    );
}
__attribute__((naked)) void _ZN10AudioQueue7IsEmptyEv() {
    __asm__ volatile (
        ".word 0xe3021714\n"
        ".word 0xe3022710\n"
        ".word 0xe7901001\n"
        ".word 0xe7900002\n"
        ".word 0xe0401001\n"
        ".word 0xe3510000\n"
        ".word 0xe2810064\n"
        ".word 0xb1a01000\n"
        ".word 0xe3a00000\n"
        ".word 0xe3510000\n"
        ".word 0x03a00001\n"
    );
}
__attribute__((naked)) void _ZN10BattleGrid15SetGridPositionEii() {
    __asm__ volatile (
        ".word 0xee012a10\n"
        ".word 0xe3043c60\n"
        ".word 0xee001a10\n"
        ".word 0xe0801003\n"
        ".word 0xf3bb0600\n"
        ".word 0xe3042c64\n"
        ".word 0xf3bb1601\n"
        ".word 0xe0800002\n"
        ".word 0xed810a00\n"
        ".word 0xed801a00\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN10BattleGrid19SetLayerSubTexturesEttt() {
    __asm__ volatile (
        ".word 0xe0800101\n"
        ".word 0xe301180c\n"
        ".word 0xe18020b1\n"
        ".word 0xe301280e\n"
        ".word 0xe18030b2\n"
    );
}
__attribute__((naked)) void _Z18IsOverMapInterfaceii() {
    __asm__ volatile (
        ".word 0xe3a02001\n"
        ".word 0xe300319a\n"
        ".word 0xe1510003\n"
        ".word 0xca000044\n"
        ".word 0xe30f2e9a\n"
        ".word 0xe3003166\n"
        ".word 0xe34f2fff\n"
        ".word 0xe0433001\n"
        ".word 0xe0812002\n"
        ".word 0xe3e0cd09\n"
        ".word 0xe3520000\n"
        ".word 0xb1a02003\n"
        ".word 0xe080300c\n"
        ".word 0xe300c241\n"
        ".word 0xe3530000\n"
        ".word 0xe04cc000\n"
        ".word 0xb1a0300c\n"
        ".word 0xe1530002\n"
        ".word 0xd0833082\n"
        ".word 0xc0823083\n"
        ".word 0xe3a02001\n"
        ".word 0xe35300fa\n"
        ".word 0xba000031\n"
        ".word 0xe2412f63\n"
        ".word 0xe2613f63\n"
        ".word 0xe3520000\n"
        ".word 0xe260cf73\n"
        ".word 0xb1a02003\n"
        ".word 0xe2403f73\n"
        ".word 0xe3530000\n"
        ".word 0xb1a0300c\n"
        ".word 0xe1530002\n"
        ".word 0xd0833082\n"
        ".word 0xc0823083\n"
        ".word 0xe3a02001\n"
        ".word 0xe3530064\n"
        ".word 0xba000023\n"
        ".word 0xe30f2e5d\n"
        ".word 0xe30031a3\n"
        ".word 0xe34f2fff\n"
        ".word 0xe0433000\n"
        ".word 0xe0802002\n"
        ".word 0xe261cf6b\n"
        ".word 0xe3520000\n"
        ".word 0xb1a02003\n"
        ".word 0xe2413f6b\n"
        ".word 0xe3530000\n"
        ".word 0xb1a0300c\n"
        ".word 0xe1520003\n"
        ".word 0xd0823083\n"
        ".word 0xc0833082\n"
        ".word 0xe3a02001\n"
        ".word 0xe353003c\n"
        ".word 0xba000012\n"
        ".word 0xe30f2ea2\n"
        ".word 0xe300315e\n"
        ".word 0xe34f2fff\n"
        ".word 0xe0812002\n"
        ".word 0xe0431001\n"
        ".word 0xe3520000\n"
        ".word 0xe3e03f76\n"
        ".word 0xb1a02001\n"
        ".word 0xe0801003\n"
        ".word 0xe3510000\n"
        ".word 0xe30031d9\n"
        ".word 0xe0430000\n"
        ".word 0xb1a01000\n"
        ".word 0xe1510002\n"
        ".word 0xd0811082\n"
        ".word 0xc0821081\n"
        ".word 0xe3a02000\n"
        ".word 0xe351003c\n"
        ".word 0xb3a02001\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN11PCamera_xia12SetViewRangeEff() {
    __asm__ volatile (
        ".word 0xe5801150\n"
        ".word 0xe5802154\n"
        ".word 0xe3a02001\n"
        ".word 0xe5c021c2\n"
        ".word 0xe5c021c0\n"
        ".word 0xe5c021c3\n"
    );
}
__attribute__((naked)) void _ZN5World9ForceWaitEv() {
    __asm__ volatile (
        ".word 0xe3a0100a\n"
        ".word 0xe3a02001\n"
        ".word 0xe580105c\n"
        ".word 0xe5c02080\n"
    );
}
__attribute__((naked)) void _ZN14UicComboButton13GetCommandKeyEv() {
    __asm__ volatile (
        ".word 0xe59010dc\n"
        ".word 0xe3a02000\n"
        ".word 0xe2411001\n"
        ".word 0xe3510001\n"
        ".word 0x959020e0\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN10UicCSBLand5ResetEv() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe3a02001\n"
        ".word 0xe5c01030\n"
        ".word 0xe5c02070\n"
        ".word 0xe5801078\n"
        ".word 0xe580107c\n"
    );
}
__attribute__((naked)) void _ZN10UicCSBLand14ShowFinishTipsEv() {
    __asm__ volatile (
        ".word 0xe3a01003\n"
        ".word 0xe3a02000\n"
        ".word 0xe580107c\n"
        ".word 0xe3a01032\n"
        ".word 0xe580206c\n"
        ".word 0xe3a02037\n"
        ".word 0xe590005c\n"
        ".word 0xe3a03d0a\n"
        ".word 0xe280901c\n"
        ".word 0xe889000e\n"
        ".word 0xe5803028\n"
    );
}
__attribute__((naked)) void _ZN10UicCSBLand13GetCommandKeyEv() {
    __asm__ volatile (
        ".word 0xe590207c\n"
        ".word 0xe3a01000\n"
        ".word 0xe3520002\n"
        ".word 0x1a000003\n"
        ".word 0xe5900084\n"
        ".word 0xe3a01065\n"
        ".word 0xe3700001\n"
        ".word 0x03a0106c\n"
        ".word 0xe1a00001\n"
    );
}
__attribute__((naked)) void _ZN7UicDots13SetCurrentDotEi() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000004\n"
        ".word 0xe5903078\n"
        ".word 0xe3a02000\n"
        ".word 0xe1530001\n"
        ".word 0xc3a02001\n"
        ".word 0xc580107c\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN7UicDots10SetNextDotEv() {
    __asm__ volatile (
        ".word 0xe590107c\n"
        ".word 0xe2811001\n"
        ".word 0xe580107c\n"
        ".word 0xe5902078\n"
        ".word 0xe1510002\n"
        ".word 0x03a01000\n"
        ".word 0x0580107c\n"
    );
}
__attribute__((naked)) void _ZN7UicDots10SetPrevDotEv() {
    __asm__ volatile (
        ".word 0xe590107c\n"
        ".word 0xe3510000\n"
        ".word 0xe2412001\n"
        ".word 0xe580207c\n"
        ".word 0x05901078\n"
        ".word 0x02411001\n"
        ".word 0x0580107c\n"
    );
}
__attribute__((naked)) void _ZN14RomanceUIScene12isResponsingEv() {
    __asm__ volatile (
        ".word 0xe590003c\n"
        ".word 0xe3a01000\n"
        ".word 0xe3500000\n"
        ".word 0x15d01154\n"
        ".word 0xe6ef0071\n"
    );
}
__attribute__((naked)) void _ZN18FlagSailModUIScene12isResponsingEv() {
    __asm__ volatile (
        ".word 0xe590105c\n"
        ".word 0xe3a02001\n"
        ".word 0xe59110b4\n"
        ".word 0xe3510000\n"
        ".word 0x1a000004\n"
        ".word 0xe5902060\n"
        ".word 0xe59200b4\n"
        ".word 0xe3a02001\n"
        ".word 0xe3500000\n"
        ".word 0x03a02000\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZNK8PVRTMat3mlERKS_() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b0c\n"
        ".word 0xed920a00\n"
        ".word 0xed921a01\n"
        ".word 0xed913a00\n"
        ".word 0xed914a03\n"
        ".word 0xf3037d10\n"
        ".word 0xf3046d11\n"
        ".word 0xed922a02\n"
        ".word 0xed915a06\n"
        ".word 0xf3058d12\n"
        ".word 0xf2076d06\n"
        ".word 0xf2066d08\n"
        ".word 0xed806a00\n"
        ".word 0xed916a01\n"
        ".word 0xed917a04\n"
        ".word 0xf306ad10\n"
        ".word 0xf3079d11\n"
        ".word 0xed918a07\n"
        ".word 0xf20a9d09\n"
        ".word 0xf308ad12\n"
        ".word 0xf2099d0a\n"
        ".word 0xed809a01\n"
        ".word 0xed919a02\n"
        ".word 0xed91aa05\n"
        ".word 0xf3090d10\n"
        ".word 0xf30a1d11\n"
        ".word 0xed91ba08\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf2000d01\n"
        ".word 0xed800a02\n"
        ".word 0xed920a03\n"
        ".word 0xed921a04\n"
        ".word 0xf303dd10\n"
        ".word 0xed922a05\n"
        ".word 0xf304cd11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf305dd12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf306dd10\n"
        ".word 0xf3090d10\n"
        ".word 0xed80ca03\n"
        ".word 0xf307cd11\n"
        ".word 0xf30a1d11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf308dd12\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf2000d01\n"
        ".word 0xed80ca04\n"
        ".word 0xed800a05\n"
        ".word 0xed920a06\n"
        ".word 0xed921a07\n"
        ".word 0xf3099d10\n"
        ".word 0xed922a08\n"
        ".word 0xf30aad11\n"
        ".word 0xf3077d11\n"
        ".word 0xf3066d10\n"
        ".word 0xf3041d11\n"
        ".word 0xf3030d10\n"
        ".word 0xf2099d0a\n"
        ".word 0xf30bad12\n"
        ".word 0xf3088d12\n"
        ".word 0xf2000d01\n"
        ".word 0xf2061d07\n"
        ".word 0xf3052d12\n"
        ".word 0xf2093d0a\n"
        ".word 0xf2011d08\n"
        ".word 0xf2000d02\n"
        ".word 0xed800a06\n"
        ".word 0xed801a07\n"
        ".word 0xed803a08\n"
        ".word 0xecbd8b0c\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
}
