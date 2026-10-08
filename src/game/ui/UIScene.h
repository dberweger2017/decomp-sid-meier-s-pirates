#pragma once

class UIControl;
class CPVRTString;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class UIScene {
public:
    UIScene * GetActiveSubUIScene();
    void SetActiveSubUIScene(const CPVRTString &name);
    UIControl * GetResponsingUIControl();
    void SetResponsingUIControl(UIControl *control);

private:
    unsigned char m_unknown_00[36];
    UIControl * m_responsingControl; // +0x24
};
