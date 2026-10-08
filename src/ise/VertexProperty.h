#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class VertexProperty {
public:
    void SetColorElementCount(int count);

private:
    unsigned char m_unknown_00[8];
    int m_colorElementCount; // +0x08
};
} // namespace ISE
