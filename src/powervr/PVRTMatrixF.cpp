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
