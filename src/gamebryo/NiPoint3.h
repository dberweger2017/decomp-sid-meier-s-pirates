#pragma once

// Observed ARMv7 representation only. FSound3D's position copies and distance/
// velocity float arithmetic establish three 4-byte components at +0/+4/+8.
// The complete original constructors, operators and math API remain unrecovered.
struct NiPoint3 {
    float x;
    float y;
    float z;
};
