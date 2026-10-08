#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicFire {
public:
    int GetFireState();

private:
    unsigned char m_unknown_00[80];
    int m_state; // +0x50
};
