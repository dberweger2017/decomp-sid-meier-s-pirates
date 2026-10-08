#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEPSysModifier {
public:
    void SetActive(bool active);

private:
    unsigned char m_unknown_00[20];
    bool m_active; // +0x14
};
} // namespace ISE
