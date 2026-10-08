#pragma once

class FxScreen;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FInterface {
public:
    void SetCurrentFxScreen(FxScreen *screen);

private:
    FxScreen * m_currentScreen; // +0x00
};
