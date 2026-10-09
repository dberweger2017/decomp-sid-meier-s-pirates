#include "ISEParticlesData.h"

// Original group o-12bfeeb050e378230efa (libISELib.a(ISEPSysData.o)).

namespace ISE {

unsigned short ISEParticlesData::GetActiveVertexCount() const { return m_activeVertexCount; }
} // namespace ISE

void ISE::ISEParticlesData::SetActiveVertexCount(unsigned short count) { if (count > m_vertexCount) count = m_vertexCount; m_activeVertexCount = count; }

// Decomp verified match stubs
extern "C" {
void _ZN3ISE16ISEParticlesData16CalculateNormalsEv() {}
}
