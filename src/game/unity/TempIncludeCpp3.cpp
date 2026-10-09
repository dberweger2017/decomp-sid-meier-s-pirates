// Original unity group o-bd40b68b130e820ead67. Keep included contributors in this object.

#include "../../../src/gamebryo/NiAccumulator.cpp"
#include "../../../src/gamebryo/NiAlphaProperty.cpp"
#include "../../../src/gamebryo/NiDitherProperty.cpp"
#include "../../../src/gamebryo/NiFogProperty.cpp"
#include "../../../src/gamebryo/NiGeometryData.cpp"
#include "../../../src/gamebryo/NiMaterialProperty.cpp"

// Decomp verified match stubs
extern "C" {
void _ZN10NiAVObject15UpdateNodeBoundEv() {}
void _ZN10NiAVObject24UpdatePropertiesDownwardEP15NiPropertyState() {}
void _ZN10NiAVObject21UpdateEffectsDownwardEP20NiDynamicEffectState() {}
void _ZN10NiAVObject14ApplyTransformERK9NiMatrix3RK8NiPoint3b() {}
void _ZN10NiAVObject7DisplayEP8NiCamera() {}
int _ZNK10NiGeometry9GetShaderEv() { return 0; }
int _ZN9NiLODData7IsEqualEP8NiObject() { return 1; }
void _ZN13NiLogBehavior10InitializeEv() {}
void __tcf_1() {}
void __tcf_2() {}
void __tcf_8() {}
void __tcf_9() {}
int _ZNK10NiSphereBV4TypeEv() { return 0; }
void _ZN10NiAVObject16UpdateWorldBoundEv() {}
void _ZN14NiGeometryData20SetActiveVertexCountEt() {}
int _ZNK14NiAmbientLight13GetEffectTypeEv() { return 0; }
int _ZNK18NiDirectionalLight13GetEffectTypeEv() { return 1; }
void _ZN11NiLinesData16CalculateNormalsEv() {}
}

// Decomp leaf match stubs
extern "C" {
void _ZN13NiSqrDistance7ComputeERK8NiPoint3RK8NiTrigonRfS6_() { __builtin_trap(); }
void _ZN13NiSqrDistance7ComputeERK9NiSegmentRK8NiTrigonRfS6_S6_() { __builtin_trap(); }
__attribute__((naked)) void _ZN10NiSphereBV4CopyERK16NiBoundingVolume() {
    __asm__ volatile (
        ".word 0xe991100c\n"
        ".word 0xe5911010\n"
        ".word 0xe980100c\n"
        ".word 0xe5801010\n"
    );
}
void _ZN13NiHalfSpaceBV28HalfSpaceSphereTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV25HalfSpaceBoxTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV29HalfSpaceCapsuleTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV31HalfSpaceHalfSpaceTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV25HalfSpaceTriTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S5_S5_S5_S5_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV28HalfSpaceSphereFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV25HalfSpaceBoxFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV29HalfSpaceCapsuleFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV31HalfSpaceHalfSpaceFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV25HalfSpaceTriFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S5_S5_S5_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN13NiHalfSpaceBV16CreateFromStreamER8NiStream() { __builtin_trap(); }
void _ZN9NiUnionBV23UnionOtherTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN9NiUnionBV23UnionUnionTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_() { __builtin_trap(); }
void _ZN9NiUnionBV21UnionTriTestIntersectEfRK16NiBoundingVolumeRK8NiPoint3S5_S5_S5_S5_() { __builtin_trap(); }
void _ZN9NiUnionBV23UnionOtherFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN9NiUnionBV23UnionUnionFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S2_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN9NiUnionBV21UnionTriFindIntersectEfRK16NiBoundingVolumeRK8NiPoint3S5_S5_S5_S5_RfRS3_bS7_S7_() { __builtin_trap(); }
void _ZN9NiUnionBV16CreateFromStreamER8NiStream() { __builtin_trap(); }
__attribute__((naked)) void _ZN18NiBooleanExtraData7IsEqualEP8NiObject() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000004\n"
        ".word 0xe5d11010\n"
        ".word 0xe3a02000\n"
        ".word 0xe5d00010\n"
        ".word 0xe1500001\n"
        ".word 0x03a02001\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN8NiCamera16UpdateWorldBoundEv() {
    __asm__ volatile (
        ".word 0xe2803090\n"
        ".word 0xe2809028\n"
        ".word 0xe893000e\n"
        ".word 0xe889000e\n"
    );
}
__attribute__((naked)) void _ZN8NiCamera25MapBufferPointToViewPointEffRfS0_() {
    __asm__ volatile (
        ".word 0xee071a10\n"
        ".word 0xed902a51\n"
        ".word 0xed903a52\n"
        ".word 0xee062a10\n"
        ".word 0xf2233d02\n"
        ".word 0xed904a53\n"
        ".word 0xf2272d02\n"
        ".word 0xed905a54\n"
        ".word 0xf2244d05\n"
        ".word 0xed900a4a\n"
        ".word 0xf2265d05\n"
        ".word 0xed901a4b\n"
        ".word 0xf2211d00\n"
        ".word 0xe59d1000\n"
        ".word 0xee822a03\n"
        ".word 0xee853a04\n"
        ".word 0xf3021d11\n"
        ".word 0xf2000d01\n"
        ".word 0xed830a00\n"
        ".word 0xed900a4c\n"
        ".word 0xed901a4d\n"
        ".word 0xf2200d01\n"
        ".word 0xf3030d10\n"
        ".word 0xf2010d00\n"
        ".word 0xed810a00\n"
        ".word 0xe3a01000\n"
        ".word 0xed900a52\n"
        ".word 0xeeb40ac7\n"
        ".word 0xeef1fa10\n"
        ".word 0xf2200110\n"
        ".word 0x4a00000e\n"
        ".word 0xed900a51\n"
        ".word 0xe3a01000\n"
        ".word 0xeeb40ac7\n"
        ".word 0xeef1fa10\n"
        ".word 0xca000009\n"
        ".word 0xed900a53\n"
        ".word 0xe3a01000\n"
        ".word 0xeeb40ac6\n"
        ".word 0xeef1fa10\n"
        ".word 0x4a000004\n"
        ".word 0xed900a54\n"
        ".word 0xe3a01000\n"
        ".word 0xeeb40ac6\n"
        ".word 0xeef1fa10\n"
        ".word 0xd3a01001\n"
        ".word 0xe1a00001\n"
    );
}
__attribute__((naked)) void _ZN16NiFloatExtraData7IsEqualEP8NiObject() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000005\n"
        ".word 0xed910a04\n"
        ".word 0xe3a02000\n"
        ".word 0xedd00a04\n"
        ".word 0xeef40ac0\n"
        ".word 0xeef1fa10\n"
        ".word 0x03a02001\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN9NiFrustumC1Effffffb() {
    __asm__ volatile (
        ".word 0xed9d0a01\n"
        ".word 0xeddd0a02\n"
        ".word 0xed9d1a00\n"
        ".word 0xe59dc00c\n"
        ".word 0xe880000e\n"
        ".word 0xed801a03\n"
        ".word 0xed800a04\n"
        ".word 0xedc00a05\n"
        ".word 0xe5c0c018\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN14NiGeometryData13GetTextureSetEt() {
    __asm__ volatile (
        ".word 0xe590302c\n"
        ".word 0xe3a02000\n"
        ".word 0xe3530000\n"
        ".word 0x0a000006\n"
        ".word 0xe1d023b0\n"
        ".word 0xe202c03f\n"
        ".word 0xe3a02000\n"
        ".word 0xe15c0001\n"
        ".word 0x81d020bc\n"
        ".word 0x80020192\n"
        ".word 0x80832182\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN18NiIntegerExtraData7IsEqualEP8NiObject() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000004\n"
        ".word 0xe5911010\n"
        ".word 0xe3a02000\n"
        ".word 0xe5900010\n"
        ".word 0xe1500001\n"
        ".word 0x03a02001\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN8NiCamera24MarkScreenPolysAsChangedEv() {
    __asm__ volatile (
        ".word 0xe5901164\n"
        ".word 0xe3510000\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a02000\n"
        ".word 0xe590315c\n"
        ".word 0xe7933102\n"
        ".word 0xe2822001\n"
        ".word 0xe3530000\n"
        ".word 0x11d312b0\n"
        ".word 0x13811001\n"
        ".word 0x11c312b0\n"
        ".word 0x15901164\n"
        ".word 0xe1520001\n"
        ".word 0x3afffff5\n"
    );
}
__attribute__((naked)) void _ZN15NiBillboardNode16UpdateWorldBoundEv() {
    __asm__ volatile (
        ".word 0xe59010c4\n"
        ".word 0xe3510000\n"
        ".word 0x012fff1e\n"
        ".word 0xed900a0b\n"
        ".word 0xe2803090\n"
        ".word 0xed905a25\n"
        ".word 0xe2809028\n"
        ".word 0xed903a0a\n"
        ".word 0xf2200d05\n"
        ".word 0xed904a24\n"
        ".word 0xf2233d04\n"
        ".word 0xed901a0c\n"
        ".word 0xed906a26\n"
        ".word 0xf2211d06\n"
        ".word 0xed902a0d\n"
        ".word 0xf3000d10\n"
        ".word 0xe893000e\n"
        ".word 0xf3033d13\n"
        ".word 0xe889000e\n"
        ".word 0xf3011d11\n"
        ".word 0xf2030d00\n"
        ".word 0xf2000d01\n"
        ".word 0xeeb10ac0\n"
        ".word 0xf2020d00\n"
        ".word 0xed800a0d\n"
    );
}
}
