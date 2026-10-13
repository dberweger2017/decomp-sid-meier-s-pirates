#pragma once
#include "ISEVector3.h"

namespace ISE {

// Original 16-byte plane view: normal at +0x00 and distance at +0x0c.
// Classification values are established by the original return branches.
class ISEPlane {
public:
    enum Classification { Positive = 0, Negative = 1, Intersecting = 2 };
    ISEPlane();
    ISEPlane(float nx, float ny, float nz, float distance);
    float GetDistance(ISEVector3 &point);
    float GetDistance(float px, float py, float pz);
    int ClassifyPoint(const ISEVector3 &center, const ISEVector3 &halfSize);
    void CreateFromTriangle(ISEVector3 &a, ISEVector3 &b, ISEVector3 &c);

    ISEVector3 m_normal; // +0x00; original constructors call the complete vector constructor.
    float m_distance; // +0x0c
};

} // namespace ISE
