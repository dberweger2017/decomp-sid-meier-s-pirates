#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEMesh {
public:
    void ShowAABB(bool show);

private:
    unsigned char m_unknown_00[64];
    bool m_showAABB; // +0x40
};
} // namespace ISE
