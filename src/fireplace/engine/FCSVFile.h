#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FCSVFile {
public:
    unsigned int GetNumRows();
    unsigned int GetNumCols();

private:
    unsigned char m_unknown_00[12];
    unsigned int m_numRows; // +0x0c
    unsigned int m_numCols; // +0x10
};
