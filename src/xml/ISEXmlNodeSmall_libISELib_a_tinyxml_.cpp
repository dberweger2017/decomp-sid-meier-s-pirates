#include "ISEXmlNode.h"

// Original compilation group o-0ae9e92e1c7f2e027e25.
const ISEXmlDocument* ISEXmlNode::ToDocument() const { return 0; }
const ISEXmlElement* ISEXmlNode::ToElement() const { return 0; }
const ISEXmlComment* ISEXmlNode::ToComment() const { return 0; }
const ISEXmlUnknown* ISEXmlNode::ToUnknown() const { return 0; }
const ISEXmlText* ISEXmlNode::ToText() const { return 0; }
const ISEXmlDeclaration* ISEXmlNode::ToDeclaration() const { return 0; }
ISEXmlDocument* ISEXmlNode::ToDocument() { return 0; }
ISEXmlElement* ISEXmlNode::ToElement() { return 0; }
ISEXmlComment* ISEXmlNode::ToComment() { return 0; }
ISEXmlUnknown* ISEXmlNode::ToUnknown() { return 0; }
ISEXmlText* ISEXmlNode::ToText() { return 0; }
ISEXmlDeclaration* ISEXmlNode::ToDeclaration() { return 0; }
