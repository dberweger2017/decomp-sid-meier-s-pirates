#pragma once

// Observed two-word instance layout from constructor, Clear and volume access.
// Original object-pool/static registration and full ownership API remain missing.
class FKnob {
public:
    FKnob();
    ~FKnob();
    void Clear();
    void SetVolume(float volume);
    float GetVolume();
    void AddKnob(FKnob *knob);
    float GetAllVolumes();

private:
    float m_volume;              // +0x00
    FKnob *m_next;               // +0x04
};
