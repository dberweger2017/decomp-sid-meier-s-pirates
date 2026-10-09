#include "UicDots.h"

// Original compilation group o-0aaf8c6270adb89d12c9.

void UicDots::Update(PVRTVec2*, PVRTVec2*) {  }

void UicDots::SetNextDot() {
    ++m_currentDot;
    if (m_currentDot == m_numDots) m_currentDot = 0;
}

void UicDots::SetPrevDot() {
    if (m_currentDot == 0) m_currentDot = m_numDots - 1;
    else --m_currentDot;
}
