#include "ISEMesh.h"

// Original group o-97ff96143a03b7b7e331 (libISELib.a(ISEMesh.o)).

namespace ISE {

void ISEMesh::ShowAABB(bool show) { m_showAABB = show; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
int _ZN3ISE7ISEMesh9IntersectEPNS_11ISEMeshNodeEPNS_6ISERayE() { return 0; }
int _ZN3ISE7ISEMesh14VectorInsideMeEPNS_11ISEMeshNodeENS_10ISEVector3EPf() { return 0; }
void _GLOBAL__I__ZN3ISE7ISEMesh15m_sbDrawWithVBOE() {}
}
