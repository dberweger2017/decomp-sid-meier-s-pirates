#include "ISEDrawListRenderUnit.h"
#include "ISEDrawList.h"

// Original group o-a07731fea5c57d908920 (libISELib.a(ISEDrawList.o)).

namespace ISE {

unsigned int ISEDrawListRenderUnit::GetVertexNum() { return m_drawList->m_vertexCount; }

ISEMaterial * ISEDrawListRenderUnit::GetMaterial() { return m_drawList->m_material; }
} // namespace ISE

#include "ISEDrawListReleaseISEDrawList.cpp"
