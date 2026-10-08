#include "ISEPSysEmitterDeclinationCtlr.h"

namespace ISE {
void ISEPSysEmitterDeclinationCtlr::SetTargetValue(float value) { m_target->m_declination = value; }
float ISEPSysEmitterDeclinationCtlr::GetTargetValue() { return m_target->m_declination; }
} // namespace ISE
