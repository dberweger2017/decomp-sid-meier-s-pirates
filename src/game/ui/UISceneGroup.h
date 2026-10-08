#pragma once

class UIControl;
class UIScene;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UISceneGroup {
public:
    UIScene * GetActiveSubUIScene();
    void SetResponsingUIControl(UIControl *control);
    UIScene * GetChild(int index);

private:
    unsigned char m_unknown_00[36];
    UIControl * m_responsingControl; // +0x24
    unsigned char m_unknown_28[20];
    UIScene * m_activeSubScene; // +0x3c
    UIScene ** m_children; // +0x40
};
