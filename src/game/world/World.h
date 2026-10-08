#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class World {
public:
    void ForceUpdate();

private:
    unsigned char m_unknown_00[92];
    int m_forceUpdateCount; // +0x5c
};
