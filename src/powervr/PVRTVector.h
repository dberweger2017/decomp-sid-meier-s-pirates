#pragma once
#include "PVRTMathTypes.h"

struct PVRTVec4 {
    float x, y, z, w;
};

struct PVRTVec3 {
    float x, y, z;
    explicit PVRTVec3(const PVRTVec4 &vector);
};

struct PVRTMat4 : PVRTMATRIXf {
    PVRTMat4 operator*(const PVRTMat4 &right) const;
    static PVRTMat4 RotationX(float angle);
    static PVRTMat4 RotationY(float angle);
    static PVRTMat4 RotationZ(float angle);
};

struct PVRTMat3 {
    float f[9];
    explicit PVRTMat3(const PVRTMat4 &matrix);
    static PVRTMat3 RotationX(float angle);
    static PVRTMat3 RotationY(float angle);
    static PVRTMat3 RotationZ(float angle);
};
