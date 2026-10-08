#include "ISEEntity.h"

// Original group o-fb9d7fdc1b359c53ebad (libISELib.a(ISEEntity.o)).

namespace ISE {

unsigned int ISEEntity::GetRenderUnitNum() { return m_renderUnitCount; }

unsigned int ISEEntity::GetPolyNum() { return m_polyCount; }

ISENode * ISEEntity::GetNodeByIndex(int index) { return m_nodes[index]; }
} // namespace ISE
