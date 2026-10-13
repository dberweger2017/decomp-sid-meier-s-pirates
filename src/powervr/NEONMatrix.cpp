#include "PVRTMathTypes.h"

// Original group o-eb8b66dc6827453889af (libOGLES2Tools.a(neon_matrix_impl.o)).
// Ordinary vector expressions
// reconstruct the four multiply/accumulate lanes; no instruction payloads.
typedef float Float4 __attribute__((vector_size(16), aligned(4), may_alias));

void NEON_Matrix4Vector4Mul(const float *matrix, const float *vector, float *result) {
    Float4 c0 = *reinterpret_cast<const Float4 *>(matrix);
    Float4 c1 = *reinterpret_cast<const Float4 *>(matrix + 4);
    Float4 c2 = *reinterpret_cast<const Float4 *>(matrix + 8);
    Float4 c3 = *reinterpret_cast<const Float4 *>(matrix + 12);
    Float4 x = {vector[0], vector[0], vector[0], vector[0]};
    Float4 y = {vector[1], vector[1], vector[1], vector[1]};
    Float4 z = {vector[2], vector[2], vector[2], vector[2]};
    Float4 w = {vector[3], vector[3], vector[3], vector[3]};
    Float4 output = c0 * x;
    output += c1 * y;
    output += c2 * z;
    output += c3 * w;
    *reinterpret_cast<Float4 *>(result) = output;
}
