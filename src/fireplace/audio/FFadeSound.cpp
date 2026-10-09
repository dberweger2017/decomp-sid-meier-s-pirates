#include "FFadeSound.h"

EFadeState FFadeSound::GetState() { return m_state; }
void FFadeSound::SetState(EFadeState state) { m_state = state; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN10FFadeSound7SetFadeEPv10EFadeStatefffff() {
    __asm__ volatile (
        ".word 0xe28d9004\n"
        ".word 0xeddd1a00\n"
        ".word 0xec990a03\n"
        ".word 0xe880000c\n"
        ".word 0xe580101c\n"
        ".word 0xe2801010\n"
        ".word 0xedc01a02\n"
        ".word 0xed800a03\n"
        ".word 0xec810a03\n"
    );
}
}
