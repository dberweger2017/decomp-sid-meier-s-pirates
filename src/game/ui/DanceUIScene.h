#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicLabel;

class DanceUIScene {
public:
    bool skipCinematic();
    void SetHeartPosAndSize(int x, int y, int size);
    void enterCinematic();

    bool endTips();

private:
    unsigned char m_unknown_00[0xac];
    UicLabel *m_tips; // +0xac; InitUIScene constructs UicLabel
    unsigned char m_unknown_b0[0x47];
    bool m_skipCinematic; // +0xf7
};
