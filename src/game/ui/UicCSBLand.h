#pragma once

// Partial declaration for direct comparison only. Do not instantiate; the
// complete hierarchy and layout remain unrecovered.
class UicCSBLand {
public:
    void Reset();

private:
    unsigned char m_unknown_00[0x30];
    unsigned char m_resetFlag30; // +0x30
    unsigned char m_unknown_31[0x3f];
    unsigned char m_resetFlag70; // +0x70
    unsigned char m_unknown_71[7];
    unsigned int m_resetWord78; // +0x78
    unsigned int m_resetWord7c; // +0x7c
};
