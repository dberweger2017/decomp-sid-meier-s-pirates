#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class UicFrameAnimation {
public:
    void UpdatePos(float x, float y);
    void Reset();

private:
    unsigned char m_unknown_00[0x64];
    unsigned int m_unknown_64;
    unsigned int m_unknown_68;
    unsigned char m_unknown_6c;
    unsigned char m_unknown_6d;
    unsigned char m_unknown_6e[2];
    float m_x; // +0x70
    float m_y; // +0x74
};
