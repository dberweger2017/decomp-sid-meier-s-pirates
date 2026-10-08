#include "ISEEditableMesh.h"

// Original group o-e3c04b8f58296776dbea (libISELib.a(ISEEditableMesh.o)).

namespace ISE {

unsigned int ISEEditableMesh::GetVertexNum() { return m_vertexCount; }

unsigned int ISEEditableMesh::GetPolyNum() { return m_polyCount; }

void * ISEEditableMesh::GetRenderUnit(int) {
    return reinterpret_cast<unsigned char *>(this) + 0xb4;
}

void * ISEEditableMesh::GetMaterial() {
    return reinterpret_cast<unsigned char *>(this) + 0x130;
}
} // namespace ISE

#include "ISEEditableMeshSmall_libISELib_a_ISEEditableMesh_.cpp"

#include "ISERenderUnitSmall_libISELib_a_ISEEditableMesh_.cpp"

#include "../recovery/abi/o-e3c04b8f58296776dbea.cpp"

#include "../recovery/leaves/o-e3c04b8f58296776dbea.cpp"
