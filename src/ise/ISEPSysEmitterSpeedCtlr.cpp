#include "ISEPSysEmitterSpeedCtlr.h"
#include "ISEPSysEmitter.h"

// Original group o-777452410bf5df8e8644 (libISELib.a(ISEPSysEmitterSpeedCtlr.o)).

namespace ISE {

void ISEPSysEmitterSpeedCtlr::SetTargetValue(float value) { m_target->m_speed = value; }

float ISEPSysEmitterSpeedCtlr::GetTargetValue() { return m_target->m_speed; }
} // namespace ISE
