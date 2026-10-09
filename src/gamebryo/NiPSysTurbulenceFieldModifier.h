#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Fail-fast bodies preserve original traps, not working implementations of
// the corresponding factories, animation or collision operations.
class NiPSysTurbulenceFieldModifier {
public:
    static NiPSysTurbulenceFieldModifier* CreateObject();
};
