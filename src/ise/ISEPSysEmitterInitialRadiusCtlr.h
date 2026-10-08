#pragma once

#include "ISEPSysEmitter.h"

namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEPSysEmitterInitialRadiusCtlr {
public:
    void SetTargetValue(float value);
    float GetTargetValue();

private:
    unsigned char m_unknown_00[72];
    ISEPSysEmitter * m_target; // +0x48
};
} // namespace ISE
