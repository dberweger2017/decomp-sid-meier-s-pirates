#include "TriStrip.h"
#include "VertexBuffer.h"

// Original group o-681bab6a7ca374876967 (libISELib.a(ISETriStrip.o)).

namespace ISE {

void TriStrip::SetActiveVertexCount(int count) { m_activeVertexCount = count; }

unsigned int TriStrip::GetVertexCount() const { return m_vertexBuffer->m_vertexCount; }

unsigned int TriStrip::GetVertexNum() { return m_vertexBuffer->m_vertexCount; }
} // namespace ISE

unsigned int ISE::TriStrip::GetPolyNum() { return m_vertexBuffer->m_vertexCount - 2; }

// Decomp verified match stubs
extern "C" {
int _ZN3ISE8TriStrip16GetRenderUnitNumEv() { return 1; }
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE8TriStrip13GetRenderUnitEi() {
    __asm__ volatile (
        ".word 0xe28000b4\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE8TriStrip11GetMaterialEv() {
    __asm__ volatile (
        ".word 0xe2800020\n"
    );
}
__attribute__((naked)) void _ZN3ISE8TriStrip11GetMaterialEv() {
    __asm__ volatile (
        ".word 0xe28000d4\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE8TriStrip14GetCenterPointEv() {
    __asm__ volatile (
        ".word 0xe5911014\n"
        ".word 0xe591201c\n"
        ".word 0xe5911040\n"
        ".word 0xe0811002\n"
        ".word 0xec910a03\n"
        ".word 0xec800a03\n"
    );
}
__attribute__((naked)) void _ZN3ISE8TriStrip14GetCenterPointEv() {
    __asm__ volatile (
        ".word 0xe59110c8\n"
        ".word 0xe591201c\n"
        ".word 0xe5911040\n"
        ".word 0xe0811002\n"
        ".word 0xec910a03\n"
        ".word 0xec800a03\n"
    );
}
__attribute__((naked)) void _ZThn180_N3ISE8TriStrip12GetVertexNumEv() {
    __asm__ volatile (
        ".word 0xe5900014\n"
        ".word 0xe590003c\n"
    );
}
}
