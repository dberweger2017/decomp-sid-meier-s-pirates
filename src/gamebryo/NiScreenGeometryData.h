#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiScreenGeometryData {
public:
    unsigned short GetActiveVertexCount() const;

private:
    unsigned char m_unknown_00[80];
    unsigned short m_activeCount; // +0x50
};
