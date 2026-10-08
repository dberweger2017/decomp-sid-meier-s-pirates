#pragma once

#include "../../gamebryo/NiPoint3.h"

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class EditablePvrmodel {
public:
    void LoadNif(const char *path);
    void GetAABB(NiPoint3 &minimum, NiPoint3 &maximum);
};
