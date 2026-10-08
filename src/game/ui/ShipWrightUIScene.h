#pragma once
#include "../../recovery/SmallFunctionTypes.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ShipWrightUIScene {
public:
    bool ReleaseUIScene();
    void setActiveUISceneFlag(bool active);

private:
    unsigned char m_unknown_00[64];
    bool m_active; // +0x40
};
