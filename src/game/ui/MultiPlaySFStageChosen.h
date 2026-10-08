#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class MultiPlaySFStageChosen {
public:
    int GetStageChosenIndex();
    bool IsChosen();

private:
    unsigned char m_unknown_00[4];
    int m_chosenIndex; // +0x04
    bool m_chosen; // +0x08
};
