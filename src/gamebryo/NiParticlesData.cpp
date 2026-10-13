#include "NiParticlesData.h"

// Original group o-6031b6c2de40a8188a31 (TempIncludeCpp4.o).

unsigned short NiParticlesData::GetActiveVertexCount() const { return m_activeCount; }

void NiParticlesData::SetActiveVertexCount(unsigned short count) {
    if (count > m_vertexCount) {
        count = m_vertexCount;
    }
    m_activeCount = count;
}
