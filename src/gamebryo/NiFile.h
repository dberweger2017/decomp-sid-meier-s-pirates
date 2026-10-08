#pragma once


// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class NiFile {
public:
    operator bool() const;

private:
    unsigned char m_unknown_00[28];
    bool m_open; // +0x1c
};
