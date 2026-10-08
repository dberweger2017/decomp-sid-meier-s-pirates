#pragma once

// Minimal ABI declarations for the recovered ApiSet method. The original
// enumeration values and PVRShellInit object layout are not reconstructed.
// This function neither inspects its arguments nor accesses object fields.
enum prefNameIntEnum { prefNameIntEnumAbiPlaceholder = 0 };

class PVRShellInit {
public:
    bool ApiSet(prefNameIntEnum preference, int value);
};
