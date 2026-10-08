#include "FxCheckBox.h"

// Recovered bodies from FireIncludeCpp.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

FxCheckBox::CheckBoxState FxCheckBox::GetState() { return m_state; }

void FxCheckBox::SetStateAnimation(CheckBoxState state, unsigned int animation) { m_stateAnimations[state] = animation; }
