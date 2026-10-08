#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FFileIO {
public:
    unsigned int GetLength() const;

private:
    unsigned char m_unknown_00[36];
    unsigned int m_length; // +0x24
};
