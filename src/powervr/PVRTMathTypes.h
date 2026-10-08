#pragma once

// Minimal float storage established by the original math helper accesses.
// These types do not claim the complete original SDK declarations.
struct PVRTVECTOR3f {
    float x, y, z;
};

struct PVRTQUATERNIONf {
    float x, y, z, w;
};

// This definition is shared across original object groups without merging them.
struct PVRTMATRIXf {
    float f[16];
};

// The original calls resolve to this named ARM definition at the named NEON_Matrix4Mul definition.
void NEON_Matrix4Mul(const float *left, const float *right, float *result);
