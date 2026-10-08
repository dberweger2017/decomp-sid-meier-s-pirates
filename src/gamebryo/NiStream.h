#pragma once

class NiObjectGroup;
class NiTexturePalette;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiStream {
public:
    NiObjectGroup * GetGroupFromID(unsigned int id);
    NiTexturePalette * GetTexturePalette() const;
    bool BackgroundLoadFinish();

private:
    unsigned char m_unknown_00[8];
    NiObjectGroup ** m_groups; // +0x08
    unsigned char m_unknown_0c[392];
    NiTexturePalette * m_texturePalette; // +0x194
    unsigned char m_unknown_198[4];
    bool m_backgroundLoadFinished; // +0x19c
};
