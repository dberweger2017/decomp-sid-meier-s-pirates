#include "MultiPlaySFCharacterChosenUIScene.h"

// Original group o-615f11e1c3ff7aa6b275 (MultiPlaySFCharacterChosenUIScene.o).

void MultiPlaySFCharacterChosenUIScene::clearDir() { m_chosenDirection = 0; }

int MultiPlaySFCharacterChosenUIScene::IsChooseDir() { return m_chosenDirection; }

bool MultiPlaySFCharacterChosenUIScene::IsChosen() { return m_chosen; }

void MultiPlaySFCharacterChosenUIScene::SetEnemyModelIndex(int index) { m_enemyModelIndex = index; }

void MultiPlaySFCharacterChosenUIScene::SetPlayerModelIndex(int index) { m_playerModelIndex = index; }

#include "MultiPlaySFCharacterChosenUISceneSmall_MultiPlaySFCharacterChosenUIScene.cpp"

#include "../../recovery/abi/o-615f11e1c3ff7aa6b275.cpp"
