#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class Pvr_NiTexture {
public:
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

private:
    unsigned char m_unknown_00[60];
    unsigned int m_width; // +0x3c
    unsigned int m_height; // +0x40
};
