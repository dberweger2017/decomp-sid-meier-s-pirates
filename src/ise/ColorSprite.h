#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ColorSprite {
public:
    void SetPos(int x, int y);

    void SetSize(unsigned int width, unsigned int height);

private:
    unsigned int m_width; // +0x00
    unsigned int m_height; // +0x04
    int m_x; // +0x08
    int m_y; // +0x0c
};
} // namespace ISE
