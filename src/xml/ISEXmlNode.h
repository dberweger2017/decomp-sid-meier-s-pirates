#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Queries reproduce observed null/self returns; complete XML parsing is absent.
class ISEXmlNode {
public:
    const ISEXmlDocument* ToDocument() const;
    const ISEXmlElement* ToElement() const;
    const ISEXmlComment* ToComment() const;
    const ISEXmlUnknown* ToUnknown() const;
    const ISEXmlText* ToText() const;
    const ISEXmlDeclaration* ToDeclaration() const;
    ISEXmlDocument* ToDocument();
    ISEXmlElement* ToElement();
    ISEXmlComment* ToComment();
    ISEXmlUnknown* ToUnknown();
    ISEXmlText* ToText();
    ISEXmlDeclaration* ToDeclaration();
};
