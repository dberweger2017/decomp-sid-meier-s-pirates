#pragma once
#include "ISEPlane.h"
#include "ISEAABB.h"

namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISECamera {
public:
    bool IsPerspective();
    void updateFrustumPlane();
    bool IsVisible(ISEAABBBox& box);

private:
    unsigned char m_unknown_00[288];
    bool m_perspective; // +0x120
    unsigned char m_unknown_121[3];
    ISEVector3 m_position; // +0x124
    ISEVector3 m_target; // +0x130
    ISEVector3 m_up; // +0x13c
    float m_fov; // +0x148; radians, tan uses half this angle.
    unsigned char m_unknown_14c[4];
    float m_near; // +0x150
    float m_far; // +0x154
    unsigned char m_unknown_158[8];
    ISEPlane m_frustum[6]; // +0x160, stride 0x10
    bool m_frustumDirty; // +0x1c0
};

// Only the embedded-camera view used by this accessor is represented.
class ISECameraMgr {
public:
    void * GetISEOrthogonalCameraPtr();
};
} // namespace ISE
