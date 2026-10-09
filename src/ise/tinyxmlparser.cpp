#include "tinyxml.h"
#include <string.h>
#include <ctype.h>

// Original group o-404381576a822b3213c8 (libISELib.a(tinyxmlparser.o)).

const ISEXmlDocument* ISEXmlDocument::ToDocument() const {
    return this;
}

ISEXmlDocument* ISEXmlDocument::ToDocument() {
    return this;
}

const char* ISEXmlBase::SkipWhiteSpace(const char* p, ISEXmlEncoding encoding) {
    if (!p || !*p) {
        return 0;
    }
    if (encoding == ISEXML_ENCODING_UTF8) {
        while (*p) {
            const unsigned char* pU = (const unsigned char*)p;
            if (*(pU + 0) == 0xef && *(pU + 1) == 0xbb && *(pU + 2) == 0xbf) {
                p += 3;
                continue;
            }
            if (*(pU + 0) == 0xef && *(pU + 1) == 0xbf && *(pU + 2) == 0xbe) {
                p += 3;
                continue;
            }
            if (*(pU + 0) == 0xef && *(pU + 1) == 0xbf && *(pU + 2) == 0xbf) {
                p += 3;
                continue;
            }
            if (IsWhiteSpace(*p)) {
                ++p;
            } else {
                break;
            }
        }
    } else {
        while (*p && IsWhiteSpace(*p)) {
            ++p;
        }
    }
    return p;
}

void ISEXmlParsingData::Stamp(const char* now, ISEXmlEncoding encoding) {
    if (!m_cursor) {
        return;
    }
    while (m_cursor < now) {
        const unsigned char* pU = (const unsigned char*)m_cursor;
        if (*pU == 0) {
            break;
        }
        if (*pU == '\n') {
            ++m_row;
            m_col = 0;
            ++m_cursor;
            continue;
        }
        if (*pU == '\r') {
            ++m_cursor;
            continue;
        }
        if (encoding == ISEXML_ENCODING_UTF8) {
            if (*pU < 0x80) {
                ++m_cursor;
            } else if (*pU < 0xe0) {
                m_cursor += 2;
            } else if (*pU < 0xf0) {
                m_cursor += 3;
            } else {
                m_cursor += 4;
            }
        } else {
            ++m_cursor;
        }
        ++m_col;
    }
}

void ISEXmlDocument::SetError(int err, const char* errorLocation, ISEXmlParsingData* prevData, ISEXmlEncoding encoding) {
    if (m_error) {
        return;
    }
    m_error = true;
    m_errorId = err;
    m_errorDesc.clear();
    if (prevData) {
        prevData->Stamp(errorLocation, encoding);
    }
}
