#pragma once


namespace ISE {
// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class ISEVector3 {
public:
    ISEVector3();
    ISEVector3(float xx, float yy, float zz);
    ~ISEVector3(); // The existing ordinary ABI leaf supplies the observed empty body.
    ISEVector3 & operator=(ISEVector3 &other);
    ISEVector3 operator+(ISEVector3 &other);
    ISEVector3 operator-(ISEVector3 &other);
    ISEVector3 operator*(float scale);
    ISEVector3 & operator+=(ISEVector3 &other);
    float DotProduct(ISEVector3 &other);
    float absDotProduct(const ISEVector3 &other);
    ISEVector3 CrossProduct(ISEVector3 &other);
    void Normalize();

    // Observed three-float value storage, also the normal member of ISEPlane.
    float x; // +0x00
    float y; // +0x04
    float z; // +0x08
};
} // namespace ISE
