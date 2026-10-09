#include "UicFrameAnimation.h"

// Original group o-b5a90cc041122ccd1748 (UicFrameAnimation.o).

void UicFrameAnimation::UpdatePos(float x, float y) { m_x = x; m_y = y; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN17UicFrameAnimation5ResetEv() {
    __asm__ volatile (
        ".word 0xe5d0106c\n"
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x03a01000\n"
        ".word 0x05c0106d\n"
        ".word 0xe5802064\n"
        ".word 0xe5802068\n"
    );
}
__attribute__((naked)) void _ZN17UicFrameAnimation15IsAnimationOverEv() {
    __asm__ volatile (
        ".word 0xe5d0206d\n"
        ".word 0xe3a01001\n"
        ".word 0xe3520000\n"
        ".word 0x0a000004\n"
        ".word 0xe5902054\n"
        ".word 0xe3a01001\n"
        ".word 0xe5900064\n"
        ".word 0xe1500002\n"
        ".word 0x33a01000\n"
        ".word 0xe1a00001\n"
    );
}
__attribute__((naked)) void _ZN17UicFrameAnimation15GetStaticSpriteEi() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000004\n"
        ".word 0xe5903054\n"
        ".word 0xe3a02000\n"
        ".word 0xe1530001\n"
        ".word 0xc5902050\n"
        ".word 0xc7922101\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN17UicFrameAnimation6SetPosEii() {
    __asm__ volatile (
        ".word 0xee001a10\n"
        ".word 0xee012a10\n"
        ".word 0xf3bb0600\n"
        ".word 0xf3bb1601\n"
        ".word 0xed800a1c\n"
        ".word 0xed801a1d\n"
        ".word 0xf2200110\n"
    );
}
}
