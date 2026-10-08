#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Queries reproduce observed null/self returns; complete XML parsing is absent.
class TiXmlNode {
public:
    const TiXmlDocument* ToDocument() const;
    const TiXmlElement* ToElement() const;
    const TiXmlComment* ToComment() const;
    const TiXmlUnknown* ToUnknown() const;
    const TiXmlText* ToText() const;
    const TiXmlDeclaration* ToDeclaration() const;
    TiXmlDocument* ToDocument();
    TiXmlElement* ToElement();
    TiXmlComment* ToComment();
    TiXmlUnknown* ToUnknown();
    TiXmlText* ToText();
    TiXmlDeclaration* ToDeclaration();
};
