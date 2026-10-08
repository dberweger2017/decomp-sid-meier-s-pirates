#include "TriStrip.h"
#include "VertexBuffer.h"

// Original group o-681bab6a7ca374876967 (libISELib.a(ISETriStrip.o)).

namespace ISE {

void TriStrip::SetActiveVertexCount(int count) { m_activeVertexCount = count; }

unsigned int TriStrip::GetVertexCount() const { return m_vertexBuffer->m_vertexCount; }

unsigned int TriStrip::GetVertexNum() { return m_vertexBuffer->m_vertexCount; }
} // namespace ISE
