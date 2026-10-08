#pragma once

#include "ISEPSysEmitter.h"

namespace ISE {
// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class ISEPSysEmitterDeclinationCtlr {
public:
    void SetTargetValue(float value);
    float GetTargetValue();

private:
    unsigned char m_unknown_00[72];
    ISEPSysEmitter * m_target; // +0x48
};
} // namespace ISE
