#pragma once

// Partial ARMv7 ABI: constructor/destructor stores establish a vptr at +0,
// accessors establish buffer +4 and length +8. Constructor stores establish
// capacity +12. Constructors, destructor and allocator behavior remain missing.
class CPVRTString {
public:
    CPVRTString();
    CPVRTString(const CPVRTString& other);
    CPVRTString(const CPVRTString& other, unsigned long pos, unsigned long n = npos);
    CPVRTString(const char *str, unsigned long length = npos);
    virtual ~CPVRTString();
    static const unsigned long npos;
    const char *c_str() const;
    bool empty() const;
    unsigned long length() const;
    unsigned long size() const;
    const char &operator[](unsigned long index) const;
    char &operator[](unsigned long index);
    CPVRTString &operator=(const CPVRTString &other);
    CPVRTString &erase(unsigned long pos = 0, unsigned long n = npos);

private:
    CPVRTString &assign(const char *buffer, unsigned long length);
    char *m_buffer;
    unsigned long m_length;
    unsigned long m_capacity;
};

CPVRTString PVRTStringGetFileExtension(const CPVRTString& str);
