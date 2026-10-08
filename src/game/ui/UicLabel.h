#pragma once

class PVRTVec2;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicLabel {
    friend class DanceUIScene;
public:
    void Update(PVRTVec2 *position, PVRTVec2 *size);
    void SetFontSize(int size);
    void SetFontColor(int color);

private:
    unsigned char m_unknown_00[0x30];
    bool m_state30; // Meaning unrecovered; DanceUIScene reads it
    unsigned char m_unknown_31[0x33];
    int m_fontSize; // +0x64
    int m_fontColor; // +0x68
};
