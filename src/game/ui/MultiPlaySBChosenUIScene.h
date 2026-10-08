#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class MultiPlaySBChosenUIScene {
public:
    void clearDir();
    int IsChooseDir();
    bool IsChosen();
    void SetEnemyModelIndex(int index);
    void SetPlayerModelIndex(int index);

private:
    unsigned char m_unknown_00[89];
    bool m_chosen; // +0x59
    unsigned char m_unknown_5a[2];
    int m_playerModelIndex; // +0x5c
    int m_enemyModelIndex; // +0x60
    int m_chosenDirection; // +0x64
};
