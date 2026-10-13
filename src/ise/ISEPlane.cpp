#include "ISEPlane.h"

// Original group o-94d8c4649937bf6c3116 (libISELib.a(ISEPlane.o)).
namespace ISE {

ISEPlane::ISEPlane() : m_normal(), m_distance(0.0f) {
    m_normal.x = m_normal.y = m_normal.z = 0.0f;
}

ISEPlane::ISEPlane(float nx, float ny, float nz, float distance)
    : m_normal(), m_distance(distance) {
    m_normal.x = nx;
    m_normal.y = ny;
    m_normal.z = nz;
}

float ISEPlane::GetDistance(ISEVector3 &point) {
    return point.DotProduct(m_normal) + m_distance;
}

float ISEPlane::GetDistance(float px, float py, float pz) {
    ISEVector3 point(px, py, pz);
    return point.DotProduct(m_normal) + m_distance;
}

int ISEPlane::ClassifyPoint(const ISEVector3 &center, const ISEVector3 &halfSize) {
    float distance;
    {
        // The original inlined reader destroys this temporary before the
        // absolute-dot call; keep that lifetime boundary explicit.
        ISEVector3 point(center.x, center.y, center.z);
        distance = point.DotProduct(m_normal) + m_distance;
    }
    float radius = m_normal.absDotProduct(halfSize);
    if (distance < -radius) return Negative;
    if (distance > radius) return Positive;
    return Intersecting;
}

void ISEPlane::CreateFromTriangle(ISEVector3 &a, ISEVector3 &b, ISEVector3 &c) {
    ISEVector3 first = b - a;
    ISEVector3 second = c - a;
    ISEVector3 normal = second.CrossProduct(first);
    m_normal = normal;
    m_normal.Normalize();
    m_distance = -a.DotProduct(m_normal);
}

} // namespace ISE
