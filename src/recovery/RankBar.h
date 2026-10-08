#pragma once
#include "ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class RankBar {
public:
    void Update(PVRTVec2*, PVRTVec2*);
};
