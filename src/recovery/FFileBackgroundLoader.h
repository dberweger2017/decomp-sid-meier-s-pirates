#pragma once
#include "SmallFunctionTypes.h"

#include "ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class FFileBackgroundLoader {
public:
    void* FindContext(void*, bool, bool);
    void DestroyPointer(void*);
};
