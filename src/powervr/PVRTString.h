#pragma once

// Partial ARMv7 ABI: constructor/destructor stores establish a vptr at +0,
// accessors establish buffer +4 and length +8. Constructor stores establish
// capacity +12. Constructors, destructor and allocator behavior remain missing.
class CPVRTString {
public:
    virtual ~CPVRTString();
    static const unsigned long npos;
    const char *c_str() const;
    bool empty() const;
    unsigned long length() const;
    unsigned long size() const;
    const char &operator[](unsigned long index) const;
    char &operator[](unsigned long index);
    CPVRTString &operator=(const CPVRTString &other);

private:
    CPVRTString &assign(const char *buffer, unsigned long length);
    char *m_buffer;
    unsigned long m_length;
    unsigned long m_capacity;
};
