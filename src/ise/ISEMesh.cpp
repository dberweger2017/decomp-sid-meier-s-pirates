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

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZNK3ISE7ISEMesh10GetPODMeshEv() {
    __asm__ volatile (
        ".word 0xe1c000d4\n"
        ".word 0xe3a020f4\n"
        ".word 0xe59110e8\n"
        ".word 0xe591101c\n"
        ".word 0xe591102c\n"
        ".word 0xe0201290\n"
    );
}
}
