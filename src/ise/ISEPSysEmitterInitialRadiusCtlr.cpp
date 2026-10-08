#include "ISEPSysEmitterInitialRadiusCtlr.h"
#include "ISEPSysEmitter.h"

// Original group o-5dc98ded87f47401fef3 (libISELib.a(ISEPSysEmitterInitialRadiusCtlr.o)).

namespace ISE {

void ISEPSysEmitterInitialRadiusCtlr::SetTargetValue(float value) { m_target->m_initialRadius = value; }

float ISEPSysEmitterInitialRadiusCtlr::GetTargetValue() { return m_target->m_initialRadius; }
} // namespace ISE
