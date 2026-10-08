#pragma once
#include "NiAVObject.h"


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiMaterialProperty : public NiProperty {
public:
    int Type();
    void SetAlpha(float value) { alpha = value; ++revision; }
    unsigned char unknown_04[0x54];
    float alpha; // observed +0x58
    unsigned int revision; // observed +0x5c
};
