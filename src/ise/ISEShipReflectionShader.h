#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
namespace ISE {
class ISEShipReflectionShader {
public:
    void SetColorPointer(int, unsigned int, int, void const*);
    void SetNormalPointer(int, unsigned int, unsigned char, int, void const*);
    void SetUVPointer(int, int, unsigned int, int, void const*);
    void SetModelViewIT(PVRTMat3 const&);
    void SetAmbient(PVRTVec4 const&);
    void SetUVMatrix(unsigned int, PVRTMat3 const&);
    void SetLightDir(unsigned int, PVRTVec3 const&);
    void SetDiffuse(unsigned int, PVRTVec3 const&);
};
}
