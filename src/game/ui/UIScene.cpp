#include "UIScene.h"

// Recovered bodies from PiratesIncludeCpp4.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

UIScene * UIScene::GetActiveSubUIScene() { return this; }

void UIScene::SetActiveSubUIScene(const CPVRTString &name) {  }

UIControl * UIScene::GetResponsingUIControl() { return m_responsingControl; }

void UIScene::SetResponsingUIControl(UIControl *control) { m_responsingControl = control; }
