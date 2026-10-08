#pragma once


namespace ISE {
// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class ISEVector3 {
public:
    ISEVector3();
    ISEVector3(float xx, float yy, float zz);
    ISEVector3 & operator=(ISEVector3 &other);

private:
    float x; // +0x00
    float y; // +0x04
    float z; // +0x08
};
} // namespace ISE
