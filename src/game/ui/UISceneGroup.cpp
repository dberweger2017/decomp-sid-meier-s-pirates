#include "UISceneGroup.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

UIScene * UISceneGroup::GetActiveSubUIScene() { return m_activeSubScene; }

void UISceneGroup::SetResponsingUIControl(UIControl *control) { m_responsingControl = control; }

UIScene * UISceneGroup::GetChild(int index) { return m_children[index]; }
