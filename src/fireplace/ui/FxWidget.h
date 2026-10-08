#pragma once

class FWidgetHandler;

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FxWidget {
public:
    void OnBegin();
    void OnEnd();
    void SetID(long id);
    void AssignWidgetHandler(FWidgetHandler *handler);
    void ParseHelp(char *help);
    char * GetText();
    void ExecuteAction();
    void ExecuteAltAction();

private:
    unsigned char m_unknown_00[236];
    long m_id; // +0xec
    unsigned char m_unknown_f0[8];
    FWidgetHandler * m_handler; // +0xf8
    unsigned char m_unknown_fc[20];
    char * m_text; // +0x110
};
