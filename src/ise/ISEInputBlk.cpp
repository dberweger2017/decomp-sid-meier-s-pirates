#include "ISEInputBlk.h"

// Original group o-90f0186157ffca78a4e3 (libISELib.a(ISEInputBlk.o)).
namespace ISE {

void * ISEInputBlk::GetTouchPoint() {
    return reinterpret_cast<unsigned char *>(this) + 0xa4;
}

} // namespace ISE
