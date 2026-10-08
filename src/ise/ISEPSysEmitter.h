#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEPSysEmitter {
public:
    unsigned char m_unknown_00[24];
    float m_speed; // +0x18
    unsigned char m_unknown_1c[4];
    float m_declination; // +0x20
    unsigned char m_unknown_24[4];
    float m_planarAngle; // +0x28
    unsigned char m_unknown_2c[20];
    float m_initialRadius; // +0x40
};
} // namespace ISE
