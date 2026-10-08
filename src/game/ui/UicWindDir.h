#pragma once

class PVRTVec2;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicWindDir {
public:
    void Update(PVRTVec2 *position, PVRTVec2 *size);
    void SetWindDirection(float direction);

private:
    unsigned char m_unknown_00[92];
    float m_windDirection; // +0x5c
};
