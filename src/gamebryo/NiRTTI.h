#pragma once


// Partial observed layout; do not instantiate. Complete size, hierarchy,
// virtual slots, unencoded return types and remaining fields are unknown.
class NiRTTI {
public:
    NiRTTI(const char *name, const NiRTTI *base);

private:
    const char * m_name; // +0x00
    const NiRTTI * m_base; // +0x04
};
