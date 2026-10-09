#pragma once

class Font;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicKeyboard {
public:
    void Reset();
    void SetTextFont(Font *font);

private:
    unsigned char m_unknown_00[0x54];
    void *m_fontOwner; // +0x54
    unsigned char m_unknown_58[9];
    bool m_active; // +0x61
    unsigned char m_unknown_62[22];
    void *m_font; // +0x78
};
