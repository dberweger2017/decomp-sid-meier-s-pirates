#pragma once

// Partial API/input declarations. Only KeyPressed's observed key slot is
// reconstructed; full object layout and enum values remain unknown.
// Do not construct this partial PVRShellInit type.
enum prefNameIntEnum { prefNameIntEnumAbiPlaceholder = 0 };

enum PVRShellKeyName { PVRShellKeyUnknown = -1 };

class PVRShellInit {
public:
    bool ApiSet(prefNameIntEnum preference, int value);
    void KeyPressed(PVRShellKeyName key);

private:
    unsigned char m_unknown_00[0x3c];
    PVRShellKeyName m_key; // +0x3c
};
