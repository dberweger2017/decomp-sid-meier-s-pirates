#include "UicFrameAnimation.h"

// Original group o-b5a90cc041122ccd1748 (UicFrameAnimation.o).

void UicFrameAnimation::UpdatePos(float x, float y) { m_x = x; m_y = y; }

void UicFrameAnimation::Reset() {
    register unsigned int zero asm("r2") = 0;
    if (m_unknown_6c == 0) m_unknown_6d = 0;
    m_unknown_64 = zero;
    m_unknown_68 = zero;
}

#include "../../recovery/abi/o-b5a90cc041122ccd1748.cpp"

#include "../../recovery/leaves/o-b5a90cc041122ccd1748.cpp"
