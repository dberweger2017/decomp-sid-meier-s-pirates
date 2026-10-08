#pragma once

#include "ButtonState.h"

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
// Reset establishes only the zero/default state; other enum values are unknown.
class UicButton {
public:
    void Reset();
    ButtonState GetButtonState();
    void SetButtonState(ButtonState state);
    void SetCommandKey(int key);
    void SetFontSize(int value);
    void SetFontColor(int value);
    void SetBackColor(int value);

private:
    unsigned char m_unknown_00[108];
    int m_fontSize; // +0x6c
    int m_fontColor; // +0x70
    int m_backColor; // +0x74
    unsigned char m_unknown_78[60];
    ButtonState m_state; // +0xb4
    int m_commandKey; // +0xb8
};
