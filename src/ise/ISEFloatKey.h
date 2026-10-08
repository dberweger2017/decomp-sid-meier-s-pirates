#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
namespace ISE {
class ISEFloatKey {
public:
    static void Interpolate(float, ISE::ISEAnimationKey const*, ISE::ISEAnimationKey const*, void*);
    static void SaveToStream(ISE::ISEParticleEntity&, ISE::ISEAnimationKey*, unsigned int);
    static void SaveBinary(ISE::ISEParticleEntity&, ISE::ISEAnimationKey*);
    static void FillDerivedVals(ISE::ISEAnimationKey*, unsigned int);
    static void Copy(ISE::ISEAnimationKey*, ISE::ISEAnimationKey const*);
    static void DeleteArray(ISE::ISEAnimationKey*);
};
}
