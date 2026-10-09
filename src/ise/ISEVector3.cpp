#include "ISEVector3.h"

namespace ISE {
ISEVector3::ISEVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {  }
ISEVector3 & ISEVector3::operator=(ISEVector3 &other) { x = other.x; y = other.y; z = other.z; return *this; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
void _ZN3ISE10ISEVector3D1Ev() {}
}
