#pragma once

#include "ButtonState.h"

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class UicComboButton {
public:
    void Reset();
    ButtonState GetButtonState();
    int GetCommandKey();
    void SetCommandKey(int key);
    void SetFontSize(int size);

private:
    unsigned char m_unknown_00[116];
    int m_fontSize; // +0x74
    unsigned char m_unknown_78[100];
    ButtonState m_state; // +0xdc
    int m_commandKey; // +0xe0
};
