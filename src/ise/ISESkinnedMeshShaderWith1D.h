#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
namespace ISE {
class ISESkinnedMeshShaderWith1D {
public:
    void SetColorPointer(int, unsigned int, int, void const*);
    void SetWorldMatrix(PVRTMat4 const&);
    void SetLightDir(unsigned int, PVRTVec3 const&);
    void SetUVMatrix(unsigned int, PVRTMat3 const&);
};
}
