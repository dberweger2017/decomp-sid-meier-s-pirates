#pragma once
#include "../recovery/SmallFunctionTypes.h"



namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEEditableMesh {
public:
    unsigned int GetRenderUnitNum();
    void * GetRenderUnit(int index);
    void * GetMaterial();
    bool UpdateUnit();
    unsigned int GetVertexNum();
    unsigned int GetPolyNum();

private:
    unsigned char m_unknown_00[252];
    unsigned int m_vertexCount; // +0xfc
    unsigned char m_unknown_100[40];
    unsigned int m_polyCount; // +0x128
};
} // namespace ISE
