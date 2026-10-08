#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class VertexBuffer {
public:
    unsigned char m_unknown_00[60];
    unsigned int m_vertexCount; // +0x3c
};
} // namespace ISE
