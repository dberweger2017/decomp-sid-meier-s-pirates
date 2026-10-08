#include "MultiPlaySBChosenUIScene.h"

// Original group o-2fd332c51ac83f2c0987 (MultiPlaySBChosenUIScene.o).

void MultiPlaySBChosenUIScene::clearDir() { m_chosenDirection = 0; }

int MultiPlaySBChosenUIScene::IsChooseDir() { return m_chosenDirection; }

bool MultiPlaySBChosenUIScene::IsChosen() { return m_chosen; }

void MultiPlaySBChosenUIScene::SetEnemyModelIndex(int index) { m_enemyModelIndex = index; }

void MultiPlaySBChosenUIScene::SetPlayerModelIndex(int index) { m_playerModelIndex = index; }

#include "MultiPlaySBChosenUISceneSmall_MultiPlaySBChosenUIScene.cpp"

#include "../../recovery/abi/o-2fd332c51ac83f2c0987.cpp"
