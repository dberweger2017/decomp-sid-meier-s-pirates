#include "UicDanceHeart.h"

// Original group o-9aa52a401ff5225611f9 (UicDanceHeart.o).

void UicDanceHeart::setFrame(int frame) { m_frame = frame; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
int _ZN13UicDanceHeart13InitUIControlEv() { return 1; }
int _ZN13UicDanceHeart16ReleaseUIControlEv() { return 1; }
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN13UicDanceHeart13playAnimationEv() {
    __asm__ volatile (
        ".word 0xe5901050\n"
        ".word 0xe3510003\n"
        ".word 0x812fff1e\n"
        ".word 0xe5d02064\n"
        ".word 0xe3520000\n"
        ".word 0x1a000006\n"
        ".word 0xe0801101\n"
        ".word 0xe5911054\n"
        ".word 0xe3510000\n"
        ".word 0x13a02001\n"
        ".word 0x15c1206d\n"
        ".word 0xe3a02001\n"
        ".word 0xe5c02064\n"
    );
}
}
