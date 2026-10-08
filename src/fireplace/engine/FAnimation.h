#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FAnimation {
public:
    const char * GetKFMModelName();

private:
    unsigned char m_unknown_00[8];
    const char * m_kfmModelName; // +0x08
};
