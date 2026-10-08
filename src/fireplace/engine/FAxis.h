#pragma once

#include "../../gamebryo/NiColor.h"

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FAxis {
public:
    bool GetShadersPossible();
    bool LoadScene();
    void SetBackgroundColor(NiColor color);
    void SetGamma(float gamma);
};
