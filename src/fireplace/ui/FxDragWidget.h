#pragma once


// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class FxDragWidget {
    friend class FxScrollbar;
public:
    unsigned int GetNumXStops();

private:
    unsigned char m_unknown_00[0xf2];
    bool m_dragging; // Thumb state observed by FxScrollbar
    unsigned char m_unknown_f3[0x19d];
    unsigned int m_numXStops; // +0x290
};
