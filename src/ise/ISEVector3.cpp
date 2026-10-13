#include "ISEVector3.h"

namespace ISE {
ISEVector3::ISEVector3() : x(0), y(0), z(0) {  }
ISEVector3::ISEVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {  }
ISEVector3 & ISEVector3::operator=(ISEVector3 &other) {
    // Preserve the observed word copies, including NaN payloads and self-copy.
    typedef unsigned int FloatWord __attribute__((may_alias));
    FloatWord* destination = reinterpret_cast<FloatWord*>(this);
    const FloatWord* source = reinterpret_cast<const FloatWord*>(&other);
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    return *this;
}

ISEVector3 ISEVector3::operator+(ISEVector3 &other) {
    ISEVector3 result;
    result.x = x + other.x;
    result.y = y + other.y;
    result.z = z + other.z;
    return result;
}

ISEVector3 ISEVector3::operator-(ISEVector3 &other) {
    ISEVector3 result;
    result.x = x - other.x;
    result.y = y - other.y;
    result.z = z - other.z;
    return result;
}

ISEVector3 ISEVector3::operator*(float scale) {
    ISEVector3 result;
    result.x = x * scale;
    result.y = y * scale;
    result.z = z * scale;
    return result;
}

ISEVector3 & ISEVector3::operator+=(ISEVector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

float ISEVector3::DotProduct(ISEVector3 &other) {
    return (x * other.x + y * other.y) + z * other.z;
}

float ISEVector3::absDotProduct(const ISEVector3 &other) {
    // Each product is rounded as float before fabs promotes it to double.
    // The original adds those magnitudes in double, then returns a float.
    return __builtin_fabs(x * other.x) + __builtin_fabs(y * other.y)
           + __builtin_fabs(z * other.z);
}

ISEVector3 ISEVector3::CrossProduct(ISEVector3 &other) {
    ISEVector3 result;
    result.x = y * other.z - z * other.y;
    result.y = z * other.x - x * other.z;
    result.z = x * other.y - y * other.x;
    return result;
}

void ISEVector3::Normalize() {
    float length = __builtin_sqrtf((x * x + y * y) + z * z);
    if (length == 0.0f) length = 1.0f;
    x /= length;
    y /= length;
    z /= length;
}
} // namespace ISE

#include "../recovery/leaves/o-24e80dfe1ffffdcc1a74.cpp"
