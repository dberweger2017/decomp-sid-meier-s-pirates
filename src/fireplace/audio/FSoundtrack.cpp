#include "FSoundtrack.h"

// Original group o-fb8215a5a1f2d4ef8e04 (FSoundtrack.o).

void FSoundtrack::SetSongGroup(const FSoundtrackGroupNode *group, int index) { m_songGroup = group; m_songGroupIndex = index; }

void FSoundtrack::SetTaperVolume(float volume) { m_taperVolume = volume; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN11FSoundtrack4InitEv() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe3e02000\n"
        ".word 0xe3a035fe\n"
        ".word 0xe5801000\n"
        ".word 0xe5802004\n"
        ".word 0xe5802008\n"
        ".word 0xe580100c\n"
        ".word 0xe5802010\n"
        ".word 0xe5802014\n"
        ".word 0xe5c01020\n"
        ".word 0xe5803024\n"
        ".word 0xe5801028\n"
        ".word 0xe580302c\n"
        ".word 0xe3a00001\n"
    );
}
__attribute__((naked)) void _ZN11FSoundtrack11SoundIsDoneEP7FISound() {
    __asm__ volatile (
        ".word 0xe590200c\n"
        ".word 0xe1520001\n"
        ".word 0x1a000004\n"
        ".word 0xe3a02000\n"
        ".word 0xe3e03000\n"
        ".word 0xe580200c\n"
        ".word 0xe5803010\n"
        ".word 0xe5803014\n"
        ".word 0xe5902000\n"
        ".word 0xe1520001\n"
        ".word 0x112fff1e\n"
        ".word 0xe3a01000\n"
        ".word 0xe3e02000\n"
        ".word 0xe8800006\n"
        ".word 0xe5802008\n"
    );
}
}
