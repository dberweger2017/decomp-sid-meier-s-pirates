#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEParticlesData {
public:
    unsigned short GetActiveVertexCount() const;

    void SetActiveVertexCount(unsigned short count);

private:
    unsigned char m_unknown_00[4];
    unsigned short m_vertexCount; // +0x04
    unsigned char m_unknown_06[0x1e];
    unsigned short m_activeVertexCount; // +0x24
};
} // namespace ISE
