#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEParticleEntity {
public:
    unsigned int GetFileVersion() const;

private:
    unsigned char m_unknown_00[444];
    unsigned int m_fileVersion; // +0x1bc
};
} // namespace ISE
