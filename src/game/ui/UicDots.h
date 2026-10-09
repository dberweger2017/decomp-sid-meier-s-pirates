#pragma once
#include "../../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class UicDots {
public:
    void Update(PVRTVec2*, PVRTVec2*);
    void SetNextDot();
    void SetPrevDot();

private:
    unsigned char m_unknown_00[0x78];
    unsigned int m_numDots; // +0x78
    unsigned int m_currentDot; // +0x7c
};
