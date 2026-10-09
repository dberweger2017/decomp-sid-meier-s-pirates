#include "MultiPlaySeaBattleChosen.h"
#include "MultiPlaySBChosenUIScene.h"

// Original group o-861bacac8f6b0b675b76 (MultiPlaySeaBattleChosen.o).

int MultiPlaySeaBattleChosen::GetPlayerChosenIndex() { return m_chosenIndex; }

bool MultiPlaySeaBattleChosen::IsChosen() { return m_chosen; }

void MultiPlaySeaBattleChosen::ClearChosenDirection() { m_scene->clearDir(); }

int MultiPlaySeaBattleChosen::GetChosenDirection() { return m_scene->IsChooseDir(); }

#include "../../recovery/abi/o-861bacac8f6b0b675b76.cpp"

#include "../../recovery/leaves/o-861bacac8f6b0b675b76.cpp"
