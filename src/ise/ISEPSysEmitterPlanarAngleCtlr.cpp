#include "ISEPSysEmitterPlanarAngleCtlr.h"
#include "ISEPSysEmitter.h"

// Original group o-314a424024475377a06a (libISELib.a(ISEPSysEmitterPlanarAngleCtlr.o)).

namespace ISE {

void ISEPSysEmitterPlanarAngleCtlr::SetTargetValue(float value) { m_target->m_planarAngle = value; }

float ISEPSysEmitterPlanarAngleCtlr::GetTargetValue() { return m_target->m_planarAngle; }
} // namespace ISE
