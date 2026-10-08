#pragma once


namespace ISE {
class ISEMaterial;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEDrawList {
public:
    unsigned char m_unknown_00[120];
    unsigned int m_vertexCount; // +0x78
    unsigned char m_unknown_7c[64];
    ISEMaterial * m_material; // +0xbc
};
} // namespace ISE
