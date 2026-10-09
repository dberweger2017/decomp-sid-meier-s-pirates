#pragma once
#include "ReleaseHookTypes.h"
#include "../fireplace/engine/FFileIO.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class FIOBufferSync {
public:
    void Flush();
    void InitBuffer();
    int Read(void*, unsigned int);
    int Seek(long, FFileIO::SeekMode);
    int Write(void const*, unsigned int);
};
