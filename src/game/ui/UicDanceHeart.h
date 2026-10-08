#pragma once
#include "../../recovery/ReleaseHookTypes.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class UicDanceHeart {
public:
    bool InitUIControl();
    bool ReleaseUIControl();
    void setFrame(int frame);

private:
    unsigned char m_unknown_00[80];
    int m_frame; // +0x50
};
