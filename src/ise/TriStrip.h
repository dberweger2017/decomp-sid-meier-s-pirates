#pragma once
#include "../recovery/SmallFunctionTypes.h"


#include "VertexBuffer.h"

namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class TriStrip {
public:
    unsigned int GetRenderUnitNum();
    void SetActiveVertexCount(int count);
    unsigned int GetVertexCount() const;
    unsigned int GetVertexNum();

    unsigned int GetPolyNum();

private:
    unsigned char m_unknown_00[200];
    VertexBuffer * m_vertexBuffer; // +0xc8
    unsigned char m_unknown_cc[4];
    int m_activeVertexCount; // +0xd0
};
} // namespace ISE
