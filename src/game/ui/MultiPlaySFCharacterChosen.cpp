#include "MultiPlaySFCharacterChosen.h"
#include "MultiPlaySFCharacterChosenUIScene.h"

// Original group o-61a2257f0bd26f37502a (MultiPlaySwordFightingChosen.o).

int MultiPlaySFCharacterChosen::GetPlayerChosenIndex() { return m_chosenIndex; }

bool MultiPlaySFCharacterChosen::IsChosen() { return m_chosen; }

void MultiPlaySFCharacterChosen::ClearChosenDirection() { m_scene->clearDir(); }

int MultiPlaySFCharacterChosen::GetChosenDirection() { return m_scene->IsChooseDir(); }
