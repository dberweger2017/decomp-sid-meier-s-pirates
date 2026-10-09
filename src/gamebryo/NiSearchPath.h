#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class NiSearchPath {
public:
    void Reset();
    void SetReferencePath(char const*);
    void SetFilePath(char const*);
    bool GetNextSearchPath(char*);
};
