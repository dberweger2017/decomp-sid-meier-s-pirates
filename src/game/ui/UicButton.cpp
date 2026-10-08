#include "UicButton.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

void UicButton::Reset() { m_state = ButtonState_Default; }

ButtonState UicButton::GetButtonState() { return m_state; }

void UicButton::SetButtonState(ButtonState state) { m_state = state; }

void UicButton::SetCommandKey(int key) { m_commandKey = key; }

void UicButton::SetFontSize(int value) { m_fontSize = value; }

void UicButton::SetFontColor(int value) { m_fontColor = value; }

void UicButton::SetBackColor(int value) { m_backColor = value; }
