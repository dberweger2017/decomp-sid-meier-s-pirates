#include "ISEEntity.h"

// Original group o-fb9d7fdc1b359c53ebad (libISELib.a(ISEEntity.o)).

namespace ISE {

unsigned int ISEEntity::GetRenderUnitNum() { return m_renderUnitCount; }

unsigned int ISEEntity::GetPolyNum() { return m_polyCount; }

ISENode * ISEEntity::GetNodeByIndex(int index) { return m_nodes[index]; }
} // namespace ISE

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE19ISEEntityRenderUnit11GetMaterialEv() {
    __asm__ volatile (
        ".word 0xe5900014\n"
        ".word 0xe2800fbd\n"
    );
}
__attribute__((naked)) void _ZN3ISE9ISEEntity13GetRenderUnitEi() {
    __asm__ volatile (
        ".word 0xe59000fc\n"
        ".word 0xe0811081\n"
        ".word 0xe0800181\n"
    );
}
__attribute__((naked)) void _ZN3ISE9ISEEntity9SetCameraEPNS_9ISECameraE() {
    __asm__ volatile (
        ".word 0xe59020e8\n"
        ".word 0xe592201c\n"
        ".word 0xe5923034\n"
        ".word 0xe3530000\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a03000\n"
        ".word 0xe590c0f4\n"
        ".word 0xe79cc103\n"
        ".word 0xe2833001\n"
        ".word 0xe58c138c\n"
        ".word 0xe592c034\n"
        ".word 0xe153000c\n"
        ".word 0x3afffff8\n"
    );
}
__attribute__((naked)) void _ZN3ISE9ISEEntity18GetMeshByNodeIndexEi() {
    __asm__ volatile (
        ".word 0xe59020e8\n"
        ".word 0xe3a03034\n"
        ".word 0xe0010391\n"
        ".word 0xe59000f0\n"
        ".word 0xe592201c\n"
        ".word 0xe5922038\n"
        ".word 0xe7921001\n"
        ".word 0xe7900101\n"
    );
}
}
