#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiTriShapeDynamicData {
public:
    unsigned short GetActiveVertexCount() const;
    unsigned short GetActiveTriangleCount() const;
    void SetActiveVertexCount(unsigned short count);
    void SetActiveTriangleCount(unsigned short count);

private:
    unsigned char m_unknown_00[76];
    unsigned short m_activeVertexCount; // +0x4c
    unsigned short m_activeTriangleCount; // +0x4e
};
