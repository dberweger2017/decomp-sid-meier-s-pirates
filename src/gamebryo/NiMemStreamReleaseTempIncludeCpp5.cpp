#include "NiMemStream.h"

// Original compilation group o-d483319aef7bde0860fd.

NiMemStream::operator bool() const { return true; }

char * NiMemStream::Str() {
    m_stringRequested = 1;
    return m_data;
}
