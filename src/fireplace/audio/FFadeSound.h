#pragma once

enum EFadeState { EFadeStateUnknown = -1 };

// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class FFadeSound {
public:
    EFadeState GetState();
    void SetState(EFadeState state);

private:
    EFadeState m_state; // +0x00
};
