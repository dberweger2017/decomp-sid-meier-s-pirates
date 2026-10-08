#pragma once

class PVRTVec2;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicBoard {
public:
    void Update(PVRTVec2 *position, PVRTVec2 *size);
};
