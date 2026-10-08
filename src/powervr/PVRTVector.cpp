#include "PVRTVector.h"
#include "PVRTMatrixF.h"

PVRTVec3::PVRTVec3(const PVRTVec4 &vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

PVRTMat3::PVRTMat3(const PVRTMat4 &matrix) {
    f[0] = matrix.f[0];
    f[1] = matrix.f[1];
    f[2] = matrix.f[2];
    f[3] = matrix.f[4];
    f[4] = matrix.f[5];
    f[5] = matrix.f[6];
    f[6] = matrix.f[8];
    f[7] = matrix.f[9];
    f[8] = matrix.f[10];
}

PVRTMat4 PVRTMat4::operator*(const PVRTMat4 &right) const {
    PVRTMat4 result;
    NEON_Matrix4Mul(right.f, f, result.f);
    return result;
}

PVRTMat4 PVRTMat4::RotationZ(float angle) {
    PVRTMat4 result;
    PVRTMatrixRotationZF(result, angle);
    return result;
}

PVRTMat3 PVRTMat3::RotationZ(float angle) {
    return PVRTMat3(PVRTMat4::RotationZ(angle));
}

PVRTMat4 PVRTMat4::RotationY(float angle) {
    PVRTMat4 result;
    PVRTMatrixRotationYF(result, angle);
    return result;
}

PVRTMat3 PVRTMat3::RotationY(float angle) {
    return PVRTMat3(PVRTMat4::RotationY(angle));
}

PVRTMat4 PVRTMat4::RotationX(float angle) {
    PVRTMat4 result;
    PVRTMatrixRotationXF(result, angle);
    return result;
}

PVRTMat3 PVRTMat3::RotationX(float angle) {
    return PVRTMat3(PVRTMat4::RotationX(angle));
}
