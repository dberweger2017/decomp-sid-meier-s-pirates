#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FxCheckBox {
public:
    // Enumerators and trailing animation-array extent remain unrecovered.
    enum CheckBoxState { StateUnknown = -1 };
    CheckBoxState GetState();
    void SetStateAnimation(CheckBoxState state, unsigned int animation);

private:
    unsigned char m_unknown_00[656];
    CheckBoxState m_state; // +0x290
    unsigned int m_stateAnimations[0]; // +0x294
};
