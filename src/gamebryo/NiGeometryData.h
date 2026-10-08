#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiGeometryData {
public:
    unsigned short GetActiveVertexCount() const;

private:
    unsigned char m_unknown_00[12];
    unsigned short m_activeVertexCount; // +0x0c
};
