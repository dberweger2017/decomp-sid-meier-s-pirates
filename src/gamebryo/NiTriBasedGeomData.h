#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiTriBasedGeomData {
public:
    unsigned short GetActiveTriangleCount() const;

private:
    unsigned char m_unknown_00[54];
    unsigned short m_activeCount; // +0x36
};
