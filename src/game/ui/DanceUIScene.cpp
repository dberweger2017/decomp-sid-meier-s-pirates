#include "DanceUIScene.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

bool DanceUIScene::skipCinematic() { return m_skipCinematic; }

void DanceUIScene::SetHeartPosAndSize(int x, int y, int size) {  }

void DanceUIScene::enterCinematic() { m_skipCinematic = false; }
