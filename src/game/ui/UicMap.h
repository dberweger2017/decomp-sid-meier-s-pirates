#pragma once

// Partial layout recovered from the short GoToCityReport method. Do not
// instantiate; the complete object size and hierarchy remain unknown.
class UicMap {
public:
    void GoToCityReport(int cityIndex);

private:
    unsigned char m_unknown_00[0xb0];
    unsigned int m_unknown_b0; // +0xb0
    unsigned char m_unknown_b4[0x4c];
    int m_cityIndex; // +0x100
    unsigned int m_unknown_104; // +0x104
};
