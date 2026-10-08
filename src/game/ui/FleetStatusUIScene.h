#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FleetStatusUIScene {
public:
    bool isResponsing();

private:
    unsigned char m_unknown_00[308];
    bool m_responsing; // +0x134
};
