#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiShader {
public:
    const char * GetName() const;

private:
    unsigned char m_unknown_00[8];
    const char * m_name; // +0x08
};
