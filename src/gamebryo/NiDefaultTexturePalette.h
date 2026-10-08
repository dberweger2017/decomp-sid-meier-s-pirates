#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class NiDefaultTexturePalette {
public:
    void SetSearchGlobalTextureList(bool search);
    bool GetSearchGlobalTextureList();

private:
    unsigned char m_unknown_00[28];
    bool m_searchGlobalTextureList; // +0x1c
};
