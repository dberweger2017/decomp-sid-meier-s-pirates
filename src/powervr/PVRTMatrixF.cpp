#include "PVRTMatrixF.h"

void PVRTMatrixIdentityF(PVRTMATRIXf &matrix) {
    matrix.f[0] = 1.0f;
    matrix.f[4] = 0.0f;
    matrix.f[8] = 0.0f;
    matrix.f[12] = 0.0f;

    matrix.f[1] = 0.0f;
    matrix.f[5] = 1.0f;
    matrix.f[9] = 0.0f;
    matrix.f[13] = 0.0f;

    matrix.f[2] = 0.0f;
    matrix.f[6] = 0.0f;
    matrix.f[10] = 1.0f;
    matrix.f[14] = 0.0f;

    matrix.f[3] = 0.0f;
    matrix.f[7] = 0.0f;
    matrix.f[11] = 0.0f;
    matrix.f[15] = 1.0f;
}

void PVRTMatrixTranslationF(PVRTMATRIXf &matrix, float x, float y, float z) {
    matrix.f[0] = 1.0f;
    matrix.f[4] = 0.0f;
    matrix.f[8] = 0.0f;
    matrix.f[1] = 0.0f;
    matrix.f[5] = 1.0f;
    matrix.f[9] = 0.0f;
    matrix.f[2] = 0.0f;
    matrix.f[6] = 0.0f;
    matrix.f[10] = 1.0f;
    matrix.f[3] = 0.0f;
    matrix.f[7] = 0.0f;
    matrix.f[11] = 0.0f;
    matrix.f[12] = x;
    matrix.f[13] = y;
    matrix.f[14] = z;
    matrix.f[15] = 1.0f;
}

void PVRTMatrixScalingF(PVRTMATRIXf &matrix, float x, float y, float z) {
    matrix.f[0] = x;
    matrix.f[4] = 0.0f;
    matrix.f[8] = 0.0f;
    matrix.f[12] = 0.0f;
    matrix.f[1] = 0.0f;
    matrix.f[5] = y;
    matrix.f[9] = 0.0f;
    matrix.f[13] = 0.0f;
    matrix.f[2] = 0.0f;
    matrix.f[6] = 0.0f;
    matrix.f[10] = z;
    matrix.f[14] = 0.0f;
    matrix.f[3] = 0.0f;
    matrix.f[7] = 0.0f;
    matrix.f[11] = 0.0f;
    matrix.f[15] = 1.0f;
}

void PVRTMatrixVec3LerpF(PVRTVECTOR3f &result, const PVRTVECTOR3f &a,
                       const PVRTVECTOR3f &b, float t) {
    result.x = a.x + t * (b.x - a.x);
    result.y = a.y + t * (b.y - a.y);
    result.z = a.z + t * (b.z - a.z);
}

void PVRTMatrixMultiplyF(PVRTMATRIXf &result, const PVRTMATRIXf &left,
                         const PVRTMATRIXf &right) {
    NEON_Matrix4Mul(left.f, right.f, result.f);
}
