#pragma once

class PVRTVec2;
class UicDanceHalo;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicDanceStep {
public:
    bool InitUIControl();
    bool ReleaseUIControl();
    void Render(PVRTVec2 *position, PVRTVec2 *size);
    void setDurationTime(int duration);
    void cleanStates();

private:
    unsigned char m_unknown_00[0x54];
    UicDanceHalo *m_halo; // +0x54; allocating constructor calls UicDanceHalo
    unsigned char m_unknown_58[12];
    unsigned int m_state64;
    unsigned char m_unknown_68[9];
    unsigned char m_state71;
};
