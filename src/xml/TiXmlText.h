#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Queries reproduce observed null/self returns; complete XML parsing is absent.
class TiXmlText {
public:
    const TiXmlText* ToText() const;
    TiXmlText* ToText();
};
