#include "ISECamera.h"
#include <math.h>

// Original group o-9cdb54dcd3500115b6d3 (libISELib.a(ISECamera.o)).

namespace ISE {

bool ISECamera::IsPerspective() { return m_perspective; }

void ISECamera::updateFrustumPlane() {
    if (!m_frustumDirty) return;
    float tangent = static_cast<float>(tan(static_cast<double>(m_fov) * 0.5));
    float nearHeight = m_near * tangent;
    float farHeight = m_far * tangent;
    const float aspect = 1.3333333730697632f; // Original literal 0x3faaaaab.
    ISEVector3 position(m_position.x, m_position.y, m_position.z);
    ISEVector3 target(m_target.x, m_target.y, m_target.z);
    ISEVector3 up(m_up.x, m_up.y, m_up.z);
    ISEVector3 forward = target - position;
    forward.Normalize();
    ISEVector3 right = forward.CrossProduct(up);
    right.Normalize();
    ISEVector3 vertical = right.CrossProduct(forward);
    ISEVector3 nearOffset = forward * m_near;
    ISEVector3 nearCenter = position + nearOffset;
    ISEVector3 farOffset = forward * m_far;
    ISEVector3 farCenter = position + farOffset;
    ISEVector3 nearVertical = vertical * nearHeight;
    ISEVector3 nearHorizontal = right * (nearHeight * aspect);
    ISEVector3 farVertical = vertical * farHeight;
    ISEVector3 farHorizontal = right * (farHeight * aspect);
    ISEVector3 ntl = (nearCenter + nearVertical) - nearHorizontal;
    ISEVector3 ntr = (nearCenter + nearVertical) + nearHorizontal;
    ISEVector3 nbl = (nearCenter - nearVertical) - nearHorizontal;
    ISEVector3 nbr = (nearCenter - nearVertical) + nearHorizontal;
    ISEVector3 ftl = (farCenter + farVertical) - farHorizontal;
    ISEVector3 ftr = (farCenter + farVertical) + farHorizontal;
    ISEVector3 fbl = (farCenter - farVertical) - farHorizontal;
    ISEVector3 fbr = (farCenter - farVertical) + farHorizontal;
    m_frustum[0].CreateFromTriangle(ntl, ntr, ftl);
    m_frustum[1].CreateFromTriangle(nbr, nbl, fbr);
    m_frustum[2].CreateFromTriangle(nbl, ntl, fbl);
    m_frustum[3].CreateFromTriangle(ntr, nbr, fbr);
    m_frustum[4].CreateFromTriangle(ntr, ntl, nbr);
    m_frustum[5].CreateFromTriangle(ftl, ftr, fbl);
    m_frustumDirty = false;
}

bool ISECamera::IsVisible(ISEAABBBox& box) {
    updateFrustumPlane();
    PVRTVec3 center = box.getCenterPoint();
    PVRTVec3 halfSize = box.getHalfSize();
    ISEVector3 point(center.x, center.y, center.z);
    ISEVector3 extents(halfSize.x, halfSize.y, halfSize.z);
    for (int i = 0; i < 6; ++i) {
        if (m_frustum[i].ClassifyPoint(point, extents) == ISEPlane::Negative)
            return false;
    }
    return true;
}

void * ISECameraMgr::GetISEOrthogonalCameraPtr() {
    return reinterpret_cast<unsigned char *>(this) + 4;
}
} // namespace ISE

#include "../recovery/abi/o-9cdb54dcd3500115b6d3.cpp"
