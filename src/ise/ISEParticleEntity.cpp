#include "ISEParticleEntity.h"

// Original group o-11c7f0138eabdedbcbfb (libISELib.a(ISEParticleEntity.o)).

namespace ISE {

unsigned int ISEParticleEntity::GetFileVersion() const { return m_fileVersion; }
} // namespace ISE

unsigned int ISE::ISEParticleEntity::GetVersion(unsigned int a, unsigned int b, unsigned int c, unsigned int d) { return (a << 24) | (b << 16) | (c << 8) | d; }

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE17ISEParticleEntity19GetObjectFromLinkIDEi() {
    __asm__ volatile (
        ".word 0xe3710001\n"
        ".word 0xe3a02000\n"
        ".word 0x15902170\n"
        ".word 0x17922101\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN3ISE17ISEParticleEntity9SetCameraEPNS_9ISECameraE() {
    __asm__ volatile (
        ".word 0xe59021c4\n"
        ".word 0xe3520000\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a03000\n"
        ".word 0xe590c16c\n"
        ".word 0xe79cc103\n"
        ".word 0xe2833001\n"
        ".word 0xe35c0000\n"
        ".word 0x159c2044\n"
        ".word 0x158210a0\n"
        ".word 0x159021c4\n"
        ".word 0xe1530002\n"
        ".word 0x3afffff6\n"
    );
}
__attribute__((naked)) void _ZN3ISE17ISEParticleEntity11ResetOnLoopEb() {
    __asm__ volatile (
        ".word 0xe5c011e4\n"
        ".word 0xe59021c4\n"
        ".word 0xe3520000\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a03000\n"
        ".word 0xe590c16c\n"
        ".word 0xe79cc103\n"
        ".word 0xe2833001\n"
        ".word 0xe35c0000\n"
        ".word 0x15cc1059\n"
        ".word 0x159021c4\n"
        ".word 0xe1530002\n"
        ".word 0x3afffff7\n"
    );
}
__attribute__((naked)) void _ZN3ISE17ISEParticleEntity8SetLayerEjPKc() {
    __asm__ volatile (
        ".word 0xe59021c4\n"
        ".word 0xe3520000\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a03000\n"
        ".word 0xe590c16c\n"
        ".word 0xe79cc103\n"
        ".word 0xe2833001\n"
        ".word 0xe35c0000\n"
        ".word 0x159c2044\n"
        ".word 0x15821040\n"
        ".word 0x159021c4\n"
        ".word 0xe1530002\n"
        ".word 0x3afffff6\n"
    );
}
}
