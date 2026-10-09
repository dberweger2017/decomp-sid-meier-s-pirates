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
