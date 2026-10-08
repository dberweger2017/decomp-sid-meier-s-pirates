#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ColorSprite {
public:
    void SetPos(int x, int y);

private:
    unsigned char m_unknown_00[8];
    int m_x; // +0x08
    int m_y; // +0x0c
};
} // namespace ISE
