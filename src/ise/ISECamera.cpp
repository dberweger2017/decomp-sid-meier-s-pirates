#include "ISECamera.h"

// Original group o-9cdb54dcd3500115b6d3 (libISELib.a(ISECamera.o)).

namespace ISE {

bool ISECamera::IsPerspective() { return m_perspective; }

void * ISECameraMgr::GetISEOrthogonalCameraPtr() {
    return reinterpret_cast<unsigned char *>(this) + 4;
}
} // namespace ISE

#include "../recovery/abi/o-9cdb54dcd3500115b6d3.cpp"
