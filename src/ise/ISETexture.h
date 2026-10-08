#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISETexture {
public:
    bool IsAlphaEnabled();

private:
    unsigned char m_unknown_00[44];
    unsigned int m_flags; // +0x2c
};
} // namespace ISE
