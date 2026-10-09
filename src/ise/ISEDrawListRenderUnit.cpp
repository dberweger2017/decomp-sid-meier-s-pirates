#include "ISEDrawListRenderUnit.h"
#include "ISEDrawList.h"

// Original group o-a07731fea5c57d908920 (libISELib.a(ISEDrawList.o)).

namespace ISE {

unsigned int ISEDrawListRenderUnit::GetVertexNum() { return m_drawList->m_vertexCount; }

ISEMaterial * ISEDrawListRenderUnit::GetMaterial() { return m_drawList->m_material; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
void _ZN3ISE11ISEDrawList15List_SetCullingEbb() {}
int _ZN3ISE21ISEDrawListRenderUnit10UpdateUnitEv() { return 1; }
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE21ISEDrawListRenderUnit14GetCenterPointEv() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe5801000\n"
        ".word 0xe5801004\n"
        ".word 0xe5801008\n"
    );
}
}
