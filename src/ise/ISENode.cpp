#include "ISENode.h"

// Original group o-f860c8ed10a6a0d791e9 (libISELib.a(ISENode.o)).

namespace ISE {

ISENode * ISENode::SetParent(ISENode *parent) { m_parent = parent; return parent; }

void * ISENode::GetName() {
    return reinterpret_cast<unsigned char *>(this) + 4;
}

void ISENode::SetEntity(ISEEntity *entity) { m_entity = entity; }
} // namespace ISE
