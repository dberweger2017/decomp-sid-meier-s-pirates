#include "NiTriShapeDynamicData.h"

// Original group o-6031b6c2de40a8188a31 (TempIncludeCpp4.o).

unsigned short NiTriShapeDynamicData::GetActiveVertexCount() const { return m_activeVertexCount; }

unsigned short NiTriShapeDynamicData::GetActiveTriangleCount() const { return m_activeTriangleCount; }

void NiTriShapeDynamicData::SetActiveVertexCount(unsigned short count) { m_activeVertexCount = count; }

void NiTriShapeDynamicData::SetActiveTriangleCount(unsigned short count) { m_activeTriangleCount = count; }
