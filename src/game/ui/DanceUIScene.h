#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class DanceUIScene {
public:
    bool skipCinematic();
    void SetHeartPosAndSize(int x, int y, int size);
    void enterCinematic();

private:
    unsigned char m_unknown_00[247];
    bool m_skipCinematic; // +0xf7
};
