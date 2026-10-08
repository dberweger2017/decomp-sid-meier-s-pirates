#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UicFire {
public:
    int GetFireState();
    void SetCannonLoadInfoL(int loadInfo, int cannonType);
    void SetCannonLoadInfoR(int loadInfo, int cannonType);

private:
    unsigned char m_unknown_00[80];
    int m_state; // +0x50
    unsigned char m_unknown_54[56];
    int m_cannonLoadInfoL; // +0x8c
    int m_cannonTypeL; // +0x90
    int m_cannonLoadInfoR; // +0x94
    int m_cannonTypeR; // +0x98
};
