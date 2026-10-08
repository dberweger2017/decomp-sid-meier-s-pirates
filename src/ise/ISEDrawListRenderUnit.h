#pragma once
#include "../recovery/SmallFunctionTypes.h"


#include "ISEDrawList.h"

namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEDrawListRenderUnit {
public:
    bool UpdateUnit();
    unsigned int GetVertexNum();
    ISEMaterial * GetMaterial();

private:
    unsigned char m_unknown_00[16];
    ISEDrawList * m_drawList; // +0x10
};
} // namespace ISE
