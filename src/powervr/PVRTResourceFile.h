#pragma once

// Partial ARMv7 ABI. Accessors establish open +4, size +8 and data +12.
// Constructor/destructor behavior and the bytes at +5..+7 remain unrecovered.
class CPVRTResourceFile {
public:
    virtual ~CPVRTResourceFile();
    bool IsOpen() const;
    unsigned long Size() const;
    const char *StringPtr() const;

private:
    bool m_open;
    unsigned char m_unknown_05[3];
    unsigned long m_size;
    const char *m_data;
};
