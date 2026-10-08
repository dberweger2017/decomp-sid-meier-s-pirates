#include "FAnimation.h"

// Recovered bodies from FireIncludeCpp.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

const char * FAnimation::GetKFMModelName() { return m_kfmModelName; }

#include "../../gamebryo/NiKFMTool.h"

const char * FAnimation::GetKFMAVObjectName() { return m_kfmTool->m_avObjectName; }
