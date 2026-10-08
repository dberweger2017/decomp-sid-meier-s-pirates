#include "UicComboButton.h"

// Original group o-0aaf8c6270adb89d12c9 (PiratesIncludeCpp.o).

void UicComboButton::Reset() { m_state = ButtonState_Default; }

ButtonState UicComboButton::GetButtonState() { return m_state; }

void UicComboButton::SetCommandKey(int key) { m_commandKey = key; }

void UicComboButton::SetFontSize(int size) { m_fontSize = size; }
