#pragma once
#include "../powervr/PVRTVector.h"

namespace ISE {

// Partial layout observed in libISELib.a(ISEAABB.o). Transform rebuilds the
// current bounds from saved extents; complete runtime integration is separate.
class ISEAABBBox {
public:
    ISEAABBBox();
    ISEAABBBox(const PVRTVec3& upper, const PVRTVec3& lower);
    void getBox(PVRTVec3& upper, PVRTVec3& lower);
    void merge(float x, float y, float z);
    PVRTVec3 getCenterPoint();
    PVRTVec3 getHalfSize();
    void saveExtents();
    void transform(const PVRTMat4& matrix);

private:
    PVRTVec3 m_upper; // +0x00; merge takes the component maxima.
    PVRTVec3 m_lower; // +0x0c; merge takes the component minima.
    PVRTVec3 m_savedUpper; // +0x18
    PVRTVec3 m_savedLower; // +0x24
    bool m_empty; // +0x30; first merged point initializes both bounds.
};

} // namespace ISE
