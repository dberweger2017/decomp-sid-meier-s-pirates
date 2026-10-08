#pragma once

// 4x4 float storage established from the original stores. Identity does not
// distinguish row-major and column-major layout; other operations are missing.
struct PVRTMATRIXf {
    float f[16];
};

void PVRTMatrixIdentityF(PVRTMATRIXf &matrix);
