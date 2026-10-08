#include "UicComboButton.h"

// Original group o-0aaf8c6270adb89d12c9 (PiratesIncludeCpp.o).

void UicComboButton::Reset() { m_state = ButtonState_Default; }

ButtonState UicComboButton::GetButtonState() { return m_state; }

int UicComboButton::GetCommandKey() {
    const unsigned int stateMinusOne = static_cast<unsigned int>(m_state) - 1;
    int commandKey = 0;
    if (stateMinusOne <= 1) commandKey = m_commandKey;
    return commandKey;
}

void UicComboButton::SetCommandKey(int key) { m_commandKey = key; }

void UicComboButton::SetFontSize(int size) { m_fontSize = size; }
