#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
namespace ISE {
class ISESkinnedMeshShaderWithVC {
public:
    void SetNormalPointer(int, unsigned int, unsigned char, int, void const*);
    void SetMatrixInvTransPalette(int, float const*);
    void SetWorldMatrix(PVRTMat4 const&);
    void SetWorldMatrixInv(PVRTMat4 const&);
    void SetDiffuse(unsigned int, PVRTVec3 const&);
    void SetLightDir(unsigned int, PVRTVec3 const&);
    void SetLightPosition(unsigned int, PVRTVec3 const&);
    void SetUVMatrix(unsigned int, PVRTMat3 const&);
    void SetAmbient(PVRTVec4 const&);
};
}
