#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class MultiPlaySFStageChosenUISceneGroup {
public:
    bool IsChosen();
    int GetChosenStageIndex();

private:
    unsigned char m_unknown_00[168];
    bool m_chosen; // +0xa8
    unsigned char m_unknown_a9[3];
    int m_chosenStageIndex; // +0xac
};
