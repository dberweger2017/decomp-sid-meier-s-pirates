#include "ISEEditableMesh.h"

// Original group o-e3c04b8f58296776dbea (libISELib.a(ISEEditableMesh.o)).

namespace ISE {

unsigned int ISEEditableMesh::GetVertexNum() { return m_vertexCount; }

unsigned int ISEEditableMesh::GetPolyNum() { return m_polyCount; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
int _ZN3ISE15ISEEditableMesh16GetRenderUnitNumEv() { return 1; }
int _ZThn180_N3ISE15ISEEditableMesh10UpdateUnitEv() { return 1; }
int _ZN3ISE15ISEEditableMesh10UpdateUnitEv() { return 1; }
int _ZN3ISE13ISERenderUnit17IsDrawListEnabledEv() { return 0; }
void _ZN3ISE13ISERenderUnit13AddToDrawListEv() {}
}
