#include "UicDanceStep.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

bool UicDanceStep::InitUIControl() { return true; }

bool UicDanceStep::ReleaseUIControl() { return true; }

void UicDanceStep::Render(PVRTVec2 *position, PVRTVec2 *size) {  }

#include "UicDanceHalo.h"

void UicDanceStep::setDurationTime(int duration) { m_halo->m_durationTime = duration; }

void UicDanceStep::cleanStates() {
    m_state64 = 0;
    m_state71 = 0;
}
