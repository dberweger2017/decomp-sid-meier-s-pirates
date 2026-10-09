#include "ISECamera.h"

// Original group o-9cdb54dcd3500115b6d3 (libISELib.a(ISECamera.o)).

namespace ISE {

bool ISECamera::IsPerspective() { return m_perspective; }
} // namespace ISE

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE9ISECamera17SetProjectionTypeEbii() {
    __asm__ volatile (
        ".word 0xe5d02120\n"
        ".word 0xe1520001\n"
        ".word 0x012fff1e\n"
        ".word 0xe3a02001\n"
        ".word 0xe5c01120\n"
        ".word 0xe5c021c2\n"
        ".word 0xe5c021c0\n"
        ".word 0xe5c021c3\n"
    );
}
__attribute__((naked)) void _ZN3ISE12ISECameraMgr25GetISEOrthogonalCameraPtrEv() {
    __asm__ volatile (
        ".word 0xe2800004\n"
    );
}
}
