#pragma once


namespace Phono2 {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class cAudioMutex {
public:
    cAudioMutex();

private:
    unsigned char m_unknown_00[44];
    bool m_locked; // +0x2c
};
} // namespace Phono2
