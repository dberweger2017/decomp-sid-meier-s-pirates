// Original unity group o-6031b6c2de40a8188a31. Keep included contributors in this object.

#include "../../../src/gamebryo/NiParticlesData.cpp"
#include "../../../src/gamebryo/NiRendererSpecificProperty.cpp"
#include "../../../src/gamebryo/NiScreenGeometryData.cpp"
#include "../../../src/gamebryo/NiShadeProperty.cpp"
#include "../../../src/gamebryo/NiSpecularProperty.cpp"
#include "../../../src/gamebryo/NiStencilProperty.cpp"
#include "../../../src/gamebryo/NiStream.cpp"
#include "../../../src/gamebryo/NiTexturingProperty.cpp"
#include "../../../src/gamebryo/NiTriBasedGeomData.cpp"
#include "../../../src/gamebryo/NiTriShapeDynamicData.cpp"

#include "../../gamebryo/NiRTTI.cpp"

#include "../../gamebryo/maps/TempIncludeCpp4Maps.cpp"

// Decomp verified match stubs
extern "C" {
void _ZN20NiScreenGeometryData16CalculateNormalsEv() {}
void _ZN8NiStream20BackgroundLoadOnExitEv() {}
void _ZN16NiTimeController12OnPreDisplayEv() {}
void _ZNK18NiTriBasedGeomData18GetTriangleIndicesEtRtS0_S0_() {}
void _ZNK18NiTriBasedGeomData12GetStripDataERtRPKtS3_Rj() {}
void _ZN22NiVertWeightsExtraData10SaveBinaryER8NiStream() {}
void __tcf_4() {}
void __tcf_5() {}
void __tcf_11() {}
void _ZN15NiParticlesData16CalculateNormalsEv() {}
void _ZN18NiTriBasedGeomData22SetActiveTriangleCountEt() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN15NiScreenLODData11GetLODIndexEi() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000003\n"
        ".word 0xe590002c\n"
        ".word 0xe1a02001\n"
        ".word 0xe1500001\n"
        ".word 0xd1a02000\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZNK14NiTriShapeData18GetTriangleIndicesEtRtS0_S0_() {
    __asm__ volatile (
        ".word 0xe590c03c\n"
        ".word 0xe0811081\n"
        ".word 0xe08cc081\n"
        ".word 0xe1dcc0b0\n"
        ".word 0xe1c2c0b0\n"
        ".word 0xe590203c\n"
        ".word 0xe0822081\n"
        ".word 0xe1d220b2\n"
        ".word 0xe1c320b0\n"
        ".word 0xe590003c\n"
        ".word 0xe59d2000\n"
        ".word 0xe0800081\n"
        ".word 0xe1d000b4\n"
        ".word 0xe1c200b0\n"
    );
}
__attribute__((naked)) void _ZNK15NiTriStripsData12GetStripDataERtRPKtS3_Rj() {
    __asm__ volatile (
        ".word 0xe1d0c3b8\n"
        ".word 0xe1c1c0b0\n"
        ".word 0xe590103c\n"
        ".word 0xe5821000\n"
        ".word 0xe5901040\n"
        ".word 0xe5831000\n"
        ".word 0xe1d013b8\n"
        ".word 0xe1d003b6\n"
        ".word 0xe59d2000\n"
        ".word 0xe0800081\n"
        ".word 0xe6ff0070\n"
        ".word 0xe5820000\n"
    );
}
__attribute__((naked)) void _ZN17NiVectorExtraData7IsEqualEP8NiObject() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000017\n"
        ".word 0xed910a04\n"
        ".word 0xe3a02000\n"
        ".word 0xedd00a04\n"
        ".word 0xeef40ac0\n"
        ".word 0xeef1fa10\n"
        ".word 0x1a000011\n"
        ".word 0xed910a05\n"
        ".word 0xe3a02000\n"
        ".word 0xedd00a05\n"
        ".word 0xeef40ac0\n"
        ".word 0xeef1fa10\n"
        ".word 0x1a00000b\n"
        ".word 0xed910a06\n"
        ".word 0xe3a02000\n"
        ".word 0xedd00a06\n"
        ".word 0xeef40ac0\n"
        ".word 0xeef1fa10\n"
        ".word 0x1a000005\n"
        ".word 0xed910a07\n"
        ".word 0xe3a02000\n"
        ".word 0xedd00a07\n"
        ".word 0xeef40ac0\n"
        ".word 0xeef1fa10\n"
        ".word 0x03a02001\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN9NiMatrix3C1ERK8NiPoint3S2_S2_() {
    __asm__ volatile (
        ".word 0xe591c000\n"
        ".word 0xe580c000\n"
        ".word 0xe591c004\n"
        ".word 0xe580c00c\n"
        ".word 0xe5911008\n"
        ".word 0xe5801018\n"
        ".word 0xe5921000\n"
        ".word 0xe5801004\n"
        ".word 0xe5921004\n"
        ".word 0xe5801010\n"
        ".word 0xe5921008\n"
        ".word 0xe580101c\n"
        ".word 0xe5931000\n"
        ".word 0xe5801008\n"
        ".word 0xe5931004\n"
        ".word 0xe5801014\n"
        ".word 0xe5931008\n"
        ".word 0xe5801020\n"
    );
}
__attribute__((naked)) void _ZNK9NiMatrix3mlERKS_() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b0c\n"
        ".word 0xed920a00\n"
        ".word 0xed921a03\n"
        ".word 0xed913a00\n"
        ".word 0xed914a01\n"
        ".word 0xf3037d10\n"
        ".word 0xf3046d11\n"
        ".word 0xed922a06\n"
        ".word 0xed915a02\n"
        ".word 0xf3058d12\n"
        ".word 0xf2076d06\n"
        ".word 0xf2066d08\n"
        ".word 0xed806a00\n"
        ".word 0xed916a03\n"
        ".word 0xed917a04\n"
        ".word 0xf306ad10\n"
        ".word 0xf3079d11\n"
        ".word 0xed918a05\n"
        ".word 0xf20a9d09\n"
        ".word 0xf308ad12\n"
        ".word 0xf2099d0a\n"
        ".word 0xed809a03\n"
        ".word 0xed919a06\n"
        ".word 0xed91aa07\n"
        ".word 0xf3090d10\n"
        ".word 0xf30a1d11\n"
        ".word 0xed91ba08\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf2000d01\n"
        ".word 0xed800a06\n"
        ".word 0xed920a01\n"
        ".word 0xed921a04\n"
        ".word 0xf303dd10\n"
        ".word 0xed922a07\n"
        ".word 0xf304cd11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf305dd12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf306dd10\n"
        ".word 0xf3090d10\n"
        ".word 0xed80ca01\n"
        ".word 0xf307cd11\n"
        ".word 0xf30a1d11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf308dd12\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf2000d01\n"
        ".word 0xed80ca04\n"
        ".word 0xed800a07\n"
        ".word 0xed920a02\n"
        ".word 0xed921a05\n"
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
        ".word 0xed800a02\n"
        ".word 0xed801a05\n"
        ".word 0xed803a08\n"
        ".word 0xecbd8b0c\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZNK9NiMatrix3mlERK8NiPoint3() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b06\n"
        ".word 0xed920a00\n"
        ".word 0xed921a01\n"
        ".word 0xed914a07\n"
        ".word 0xed915a06\n"
        ".word 0xf3044d11\n"
        ".word 0xf3055d10\n"
        ".word 0xed917a04\n"
        ".word 0xed91aa03\n"
        ".word 0xf3077d11\n"
        ".word 0xed918a01\n"
        ".word 0xf30aad10\n"
        ".word 0xf3081d11\n"
        ".word 0xed922a02\n"
        ".word 0xf2054d04\n"
        ".word 0xed915a00\n"
        ".word 0xf3050d10\n"
        ".word 0xed913a08\n"
        ".word 0xed916a05\n"
        ".word 0xf3033d12\n"
        ".word 0xed919a02\n"
        ".word 0xf3066d12\n"
        ".word 0xf3092d12\n"
        ".word 0xf2000d01\n"
        ".word 0xf20a1d07\n"
        ".word 0xf2043d03\n"
        ".word 0xf2000d02\n"
        ".word 0xf2011d06\n"
        ".word 0xed800a00\n"
        ".word 0xed801a01\n"
        ".word 0xed803a02\n"
        ".word 0xecbd8b06\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZmlRK8NiPoint3RK9NiMatrix3() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b06\n"
        ".word 0xed923a04\n"
        ".word 0xed926a00\n"
        ".word 0xed927a01\n"
        ".word 0xed928a02\n"
        ".word 0xed929a03\n"
        ".word 0xed92aa05\n"
        ".word 0xed910a00\n"
        ".word 0xed911a01\n"
        ".word 0xf3007d17\n"
        ".word 0xf3013d13\n"
        ".word 0xed924a07\n"
        ".word 0xf3019d19\n"
        ".word 0xed925a08\n"
        ".word 0xf3006d16\n"
        ".word 0xed912a02\n"
        ".word 0xf3011d1a\n"
        ".word 0xf3000d18\n"
        ".word 0xf3024d14\n"
        ".word 0xf3025d15\n"
        ".word 0xf2073d03\n"
        ".word 0xf2000d01\n"
        ".word 0xed921a06\n"
        ".word 0xf3021d11\n"
        ".word 0xf2062d09\n"
        ".word 0xf2000d05\n"
        ".word 0xf2021d01\n"
        ".word 0xf2032d04\n"
        ".word 0xed801a00\n"
        ".word 0xed802a01\n"
        ".word 0xed800a02\n"
        ".word 0xecbd8b06\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZNK11NiTransform6InvertERS_() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b08\n"
        ".word 0xe2802004\n"
        ".word 0xeef76a00\n"
        ".word 0xec920a03\n"
        ".word 0xed902a04\n"
        ".word 0xed903a05\n"
        ".word 0xed904a06\n"
        ".word 0xed905a07\n"
        ".word 0xed906a00\n"
        ".word 0xed907a08\n"
        ".word 0xed816a00\n"
        ".word 0xed811a01\n"
        ".word 0xed814a02\n"
        ".word 0xed810a03\n"
        ".word 0xed812a04\n"
        ".word 0xed815a05\n"
        ".word 0xedc10a06\n"
        ".word 0xed813a07\n"
        ".word 0xed817a08\n"
        ".word 0xed908a0c\n"
        ".word 0xee868a88\n"
        ".word 0xed818a0c\n"
        ".word 0xedd06a09\n"
        ".word 0xed909a0a\n"
        ".word 0xee26bac6\n"
        ".word 0xed90aa0b\n"
        ".word 0xf3011d19\n"
        ".word 0xf3033d19\n"
        ".word 0xf3022d19\n"
        ".word 0xf3044d1a\n"
        ".word 0xf3077d1a\n"
        ".word 0xf22b1d01\n"
        ".word 0xee26bae0\n"
        ".word 0xee260ac0\n"
        ".word 0xf3055d1a\n"
        ".word 0xf2211d04\n"
        ".word 0xf22b3d03\n"
        ".word 0xf2200d02\n"
        ".word 0xf3011d18\n"
        ".word 0xf2232d07\n"
        ".word 0xf2200d05\n"
        ".word 0xed811a09\n"
        ".word 0xf3022d18\n"
        ".word 0xf3000d18\n"
        ".word 0xed810a0a\n"
        ".word 0xed812a0b\n"
        ".word 0xecbd8b08\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZNK9NiMatrix314TransposeTimesERKS_() {
    __asm__ volatile (
        ".word 0xe52d7004\n"
        ".word 0xe1a0700d\n"
        ".word 0xed2d8b0c\n"
        ".word 0xed920a00\n"
        ".word 0xed921a03\n"
        ".word 0xed913a00\n"
        ".word 0xed914a03\n"
        ".word 0xf3037d10\n"
        ".word 0xf3046d11\n"
        ".word 0xed922a06\n"
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
        ".word 0xed809a03\n"
        ".word 0xed919a02\n"
        ".word 0xed91aa05\n"
        ".word 0xf3090d10\n"
        ".word 0xf30a1d11\n"
        ".word 0xed91ba08\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf2000d01\n"
        ".word 0xed800a06\n"
        ".word 0xed920a01\n"
        ".word 0xed921a04\n"
        ".word 0xf303dd10\n"
        ".word 0xed922a07\n"
        ".word 0xf304cd11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf305dd12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf306dd10\n"
        ".word 0xf3090d10\n"
        ".word 0xed80ca01\n"
        ".word 0xf307cd11\n"
        ".word 0xf30a1d11\n"
        ".word 0xf20dcd0c\n"
        ".word 0xf308dd12\n"
        ".word 0xf2000d01\n"
        ".word 0xf30b1d12\n"
        ".word 0xf20ccd0d\n"
        ".word 0xf2000d01\n"
        ".word 0xed80ca04\n"
        ".word 0xed800a07\n"
        ".word 0xed920a02\n"
        ".word 0xed921a05\n"
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
        ".word 0xed800a02\n"
        ".word 0xed801a05\n"
        ".word 0xed803a08\n"
        ".word 0xecbd8b0c\n"
        ".word 0xe49d7004\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN11NiParticles20CalculateConsistencyEb() {
    __asm__ volatile (
        ".word 0xe59000bc\n"
        ".word 0xe3a02008\n"
        ".word 0xe1d013b2\n"
        ".word 0xe7df1612\n"
        ".word 0xe1c013b2\n"
    );
}
__attribute__((naked)) void _ZN8NiStream18GetNumberOfLinkIDsEv() {
    __asm__ volatile (
        ".word 0xe5901180\n"
        ".word 0xe2812001\n"
        ".word 0xe5802180\n"
        ".word 0xe5900174\n"
        ".word 0xe7900101\n"
    );
}
__attribute__((naked)) void _ZN8NiStream19GetObjectFromLinkIDEv() {
    __asm__ volatile (
        ".word 0xe5901170\n"
        ".word 0xe2812001\n"
        ".word 0xe5802170\n"
        ".word 0xe5902164\n"
        ".word 0xe7921101\n"
        ".word 0xe3a02000\n"
        ".word 0xe3710001\n"
        ".word 0x15902130\n"
        ".word 0x17922101\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN15NiParticlesData20SetActiveVertexCountEt() {
    __asm__ volatile (
        ".word 0xe1d020bc\n"
        ".word 0xe1520001\n"
        ".word 0x31a01002\n"
        ".word 0xe1c013bc\n"
    );
}
__attribute__((naked)) void _ZNK12NiPointLight13GetEffectTypeEv() {
    __asm__ volatile (
        ".word 0xe3a00002\n"
    );
}
__attribute__((naked)) void _ZN21NiVertexColorProperty4TypeEv() {
    __asm__ volatile (
        ".word 0xe3a00009\n"
    );
}
__attribute__((naked)) void _ZN19NiWireframeProperty4TypeEv() {
    __asm__ volatile (
        ".word 0xe3a0000a\n"
    );
}
__attribute__((naked)) void _ZN17NiZBufferProperty4TypeEv() {
    __asm__ volatile (
        ".word 0xe3a0000b\n"
    );
}
__attribute__((naked)) void _ZNK11NiSpotLight13GetEffectTypeEv() {
    __asm__ volatile (
        ".word 0xe3a00003\n"
    );
}
__attribute__((naked)) void _ZNK15NiTextureEffect13GetEffectTypeEv() {
    __asm__ volatile (
        ".word 0xe3a00004\n"
    );
}
}
