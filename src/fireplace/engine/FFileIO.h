#pragma once
#include "../../recovery/ReleaseHookTypes.h"



// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FFileIO {
public:
    enum SeekMode { SeekModeUnknown = -1 }; // Enumerator values unrecovered.
    int Write(void const*, unsigned int);
    static bool Delete(char const*);
    unsigned int GetLength() const;
    bool IsOpen() const;

private:
    unsigned char m_unknown_00[4];
    void *m_file; // +0x04; file handle representation unproven
    unsigned char m_unknown_08[0x1c];
    unsigned int m_length; // +0x24
};
