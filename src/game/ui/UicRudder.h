#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class UicRudder {
public:
    void SetTargetWorldPos(int x, int y, int z);

private:
    unsigned char m_unknown_00[188];
    int m_targetWorldPosition[3]; // +0xbc
};
