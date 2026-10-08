#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISECamera {
public:
    bool IsPerspective();

private:
    unsigned char m_unknown_00[288];
    bool m_perspective; // +0x120
};
} // namespace ISE
