#include "ISEEditableMesh.h"

// Original group o-e3c04b8f58296776dbea (libISELib.a(ISEEditableMesh.o)).

namespace ISE {

unsigned int ISEEditableMesh::GetVertexNum() { return m_vertexCount; }

unsigned int ISEEditableMesh::GetPolyNum() { return m_polyCount; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
int _ZN3ISE15ISEEditableMesh16GetRenderUnitNumEv() { return 1; }
int _ZThn180_N3ISE15ISEEditableMesh10UpdateUnitEv() { return 1; }
int _ZN3ISE15ISEEditableMesh10UpdateUnitEv() { return 1; }
int _ZN3ISE13ISERenderUnit17IsDrawListEnabledEv() { return 0; }
void _ZN3ISE13ISERenderUnit13AddToDrawListEv() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE15ISEEditableMesh13GetRenderUnitEi() {
    __asm__ volatile (
        ".word 0xe28000b4\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE15ISEEditableMesh12GetVertexNumEv() {
    __asm__ volatile (
        ".word 0xe5900048\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE15ISEEditableMesh11GetMaterialEv() {
    __asm__ volatile (
        ".word 0xe280007c\n"
    );
}
__attribute__((naked)) void _ZN3ISE15ISEEditableMesh11GetMaterialEv() {
    __asm__ volatile (
        ".word 0xe2800e13\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE15ISEEditableMesh14GetCenterPointEv() {
    __asm__ volatile (
        ".word 0xe591104c\n"
        ".word 0xe891000c\n"
        ".word 0xe5911008\n"
        ".word 0xe880000c\n"
        ".word 0xe5801008\n"
    );
}
__attribute__((naked)) void _ZN3ISE15ISEEditableMesh14GetCenterPointEv() {
    __asm__ volatile (
        ".word 0xe5911100\n"
        ".word 0xe891000c\n"
        ".word 0xe5911008\n"
        ".word 0xe880000c\n"
        ".word 0xe5801008\n"
    );
}
__attribute__((naked)) void _ZN3ISE15ISERenderObject6SetPosERK8PVRTVec3() {
    __asm__ volatile (
        ".word 0xe891000c\n"
        ".word 0xe5911008\n"
        ".word 0xe5802008\n"
        ".word 0xe3a02001\n"
        ".word 0xe580300c\n"
        ".word 0xe5801010\n"
        ".word 0xe5c020ac\n"
    );
}
__attribute__((naked)) void _ZN3ISE15ISERenderObject6SetPosEfff() {
    __asm__ volatile (
        ".word 0xe2809008\n"
        ".word 0xe889000e\n"
        ".word 0xe3a02001\n"
        ".word 0xe5c020ac\n"
    );
}
}
