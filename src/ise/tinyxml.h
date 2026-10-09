#pragma once

#include <stddef.h>

enum ISEXmlEncoding {
    ISEXML_ENCODING_UNKNOWN = 0,
    ISEXML_ENCODING_UTF8 = 1,
    ISEXML_ENCODING_LEGACY = 2
};

class ISEXmlString {
public:
    ISEXmlString();
    ~ISEXmlString();
    void append(const char* str, size_t len);
    void append(const char* str);
    const char* c_str() const;
    void clear();

private:
    char* m_data;
};

class ISEXmlParsingData {
public:
    ISEXmlParsingData(const char* start, int num, int row, int col);
    void Stamp(const char* now, ISEXmlEncoding encoding);

    const char* Cursor() const { return m_cursor; }

private:
    const char* m_cursor;
    int m_row;
    int m_col;
};

class ISEXmlNode;
class ISEXmlElement;
class ISEXmlComment;
class ISEXmlUnknown;
class ISEXmlAttribute;
class ISEXmlDocument;
class ISEXmlText;
class ISEXmlDeclaration;

class ISEXmlBase {
public:
    virtual ~ISEXmlBase() {}
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding) = 0;

    static const char* SkipWhiteSpace(const char* p, ISEXmlEncoding encoding);
    static const char* ReadText(const char* in, ISEXmlString* text, bool ignoreWhite, const char* endTag, bool ignoreCase, ISEXmlEncoding encoding);
    static const char* GetEntity(const char* in, char* value, int* length, ISEXmlEncoding encoding);

    static bool IsWhiteSpace(char c) {
        return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    }
};

class ISEXmlAttribute : public ISEXmlBase {
public:
    ISEXmlAttribute();
    virtual ~ISEXmlAttribute();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);

    const char* Name() const;
    const char* Value() const;

private:
    ISEXmlDocument* m_document;
    ISEXmlString m_name;
    ISEXmlString m_value;
    ISEXmlAttribute* m_prev;
    ISEXmlAttribute* m_next;
};

class ISEXmlNode : public ISEXmlBase {
public:
    virtual ~ISEXmlNode();
    ISEXmlNode* Identify(const char* start, ISEXmlEncoding encoding);

    virtual ISEXmlElement* ToElement() { return 0; }
    virtual const ISEXmlElement* ToElement() const { return 0; }
    virtual ISEXmlDocument* ToDocument() { return 0; }
    virtual const ISEXmlDocument* ToDocument() const { return 0; }

protected:
    ISEXmlNode* m_parent;
    int m_type;
    ISEXmlNode* m_firstChild;
    ISEXmlNode* m_lastChild;
    ISEXmlString m_value;
    ISEXmlNode* m_prev;
    ISEXmlNode* m_next;
};

class ISEXmlElement : public ISEXmlNode {
public:
    ISEXmlElement(const char* val);
    virtual ~ISEXmlElement();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
    const char* ReadValue(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);

    virtual ISEXmlElement* ToElement() { return this; }
    virtual const ISEXmlElement* ToElement() const { return this; }

private:
    ISEXmlAttribute* m_firstAttribute;
    ISEXmlAttribute* m_lastAttribute;
};

class ISEXmlComment : public ISEXmlNode {
public:
    ISEXmlComment();
    virtual ~ISEXmlComment();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
};

class ISEXmlText : public ISEXmlNode {
public:
    ISEXmlText(const char* val);
    virtual ~ISEXmlText();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
};

class ISEXmlDeclaration : public ISEXmlNode {
public:
    ISEXmlDeclaration();
    virtual ~ISEXmlDeclaration();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
};

class ISEXmlUnknown : public ISEXmlNode {
public:
    ISEXmlUnknown();
    virtual ~ISEXmlUnknown();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
};

class ISEXmlDocument : public ISEXmlNode {
public:
    ISEXmlDocument();
    virtual ~ISEXmlDocument();
    virtual const char* Parse(const char* p, ISEXmlParsingData* data, ISEXmlEncoding encoding);
    void SetError(int err, const char* errorLocation, ISEXmlParsingData* prevData, ISEXmlEncoding encoding);

    virtual ISEXmlDocument* ToDocument();
    virtual const ISEXmlDocument* ToDocument() const;

private:
    bool m_error;
    int m_errorId;
    ISEXmlString m_errorDesc;
    int m_errorLocationRow;
    int m_errorLocationCol;
};
