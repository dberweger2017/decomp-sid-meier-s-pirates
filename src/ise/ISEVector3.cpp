#include "ISEVector3.h"

namespace ISE {
ISEVector3::ISEVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {  }
ISEVector3 & ISEVector3::operator=(ISEVector3 &other) { x = other.x; y = other.y; z = other.z; return *this; }
} // namespace ISE

#include "../recovery/leaves/o-24e80dfe1ffffdcc1a74.cpp"
