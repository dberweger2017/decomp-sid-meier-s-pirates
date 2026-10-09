#pragma once
#include "../../recovery/ReleaseHookTypes.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class TopTenPiratesUIScene {
public:
    bool ReleaseUIScene();
    bool isResponsing();

private:
    unsigned char m_unknown_00[388];
    bool m_responsing; // +0x184
};
