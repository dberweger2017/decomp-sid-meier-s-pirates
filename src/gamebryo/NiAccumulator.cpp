#include "NiAccumulator.h"

// Original group o-bd40b68b130e820ead67 (TempIncludeCpp3.o).

void NiAccumulator::FinishAccumulating() { m_camera = 0; }

bool NiAccumulator::StartAccumulating(NiCamera *camera) { if (m_camera) return false; m_camera = camera; return true; }
