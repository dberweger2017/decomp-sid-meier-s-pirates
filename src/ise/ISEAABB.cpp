// Original group o-f23326dbfbcca1360d5d (libISELib.a(ISEAABB.o)).
#include "ISEAABB.h"

namespace ISE {

ISEAABBBox::ISEAABBBox()
{
    m_empty = true;
    m_upper = PVRTVec3(0.0f, 0.0f, 0.0f);
    m_lower = m_upper;
}

ISEAABBBox::ISEAABBBox(const PVRTVec3& upper, const PVRTVec3& lower)
    : m_upper(upper), m_lower(lower), m_empty(true) {}

void ISEAABBBox::getBox(PVRTVec3& upper, PVRTVec3& lower) {
    upper = m_upper;
    lower = m_lower;
}

void ISEAABBBox::merge(float x, float y, float z) {
    if (m_empty) {
        m_upper.x = m_lower.x = x;
        m_upper.y = m_lower.y = y;
        m_upper.z = m_lower.z = z;
        m_empty = false;
    } else {
        if (m_upper.x < x) m_upper.x = x;
        if (m_upper.y < y) m_upper.y = y;
        if (m_upper.z < z) m_upper.z = z;
        if (m_lower.x > x) m_lower.x = x;
        if (m_lower.y > y) m_lower.y = y;
        if (m_lower.z > z) m_lower.z = z;
    }
}

PVRTVec3 ISEAABBBox::getCenterPoint() {
    return PVRTVec3((m_upper.x + m_lower.x) * 0.5f,
                   (m_upper.y + m_lower.y) * 0.5f,
                   (m_upper.z + m_lower.z) * 0.5f);
}

PVRTVec3 ISEAABBBox::getHalfSize() {
    return PVRTVec3((m_upper.x - m_lower.x) * 0.5f,
                   (m_upper.y - m_lower.y) * 0.5f,
                   (m_upper.z - m_lower.z) * 0.5f);
}

void ISEAABBBox::saveExtents() {
    m_savedUpper = m_upper;
    m_savedLower = m_lower;
}

void ISEAABBBox::transform(const PVRTMat4& matrix) {
    if (m_empty) return;
    float upperX = m_savedUpper.x;
    float upperY = m_savedUpper.y;
    float upperZ = m_savedUpper.z;
    float lowerX = m_savedLower.x;
    float lowerY = m_savedLower.y;
    float lowerZ = m_savedLower.z;
    m_empty = true;
    // Observed corner order: LLL, LLU, LUU, LUL, UUL, UUU, ULU, ULL.
    float point[4] = {lowerX, lowerY, lowerZ, 1.0f};
    float transformed[4];
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[2] = upperZ;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[1] = upperY;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[2] = lowerZ;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[0] = upperX;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[2] = upperZ;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[1] = lowerY;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
    point[2] = lowerZ;
    NEON_Matrix4Vector4Mul(matrix.f, point, transformed);
    merge(transformed[0], transformed[1], transformed[2]);
}

} // namespace ISE
