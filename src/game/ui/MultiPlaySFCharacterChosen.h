#pragma once

class MultiPlaySFCharacterChosenUIScene;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class MultiPlaySFCharacterChosen {
public:
    int GetPlayerChosenIndex();
    bool IsChosen();
    void ClearChosenDirection();
    int GetChosenDirection();

private:
    unsigned char m_unknown_00[4];
    int m_chosenIndex; // +0x04
    unsigned char m_unknown_08[4];
    bool m_chosen; // +0x0c
    unsigned char m_unknown_0d[0x0b];
    MultiPlaySFCharacterChosenUIScene *m_scene; // +0x18
};
