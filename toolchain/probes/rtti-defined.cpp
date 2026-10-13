// Diagnostic translation-unit-context variant, NOT recovered original types.
// A same-unit definition tests direct addressing; initializer semantics and
// original static initialization remain unproven.
struct NiRTTI { const char *name; const NiRTTI *base; };
class NiObject {
public:
    static const NiRTTI m_RTTI;
    const NiRTTI *GetRTTI() const;
};
const NiRTTI NiObject::m_RTTI = { "NiObject", 0 };
const NiRTTI *NiObject::GetRTTI() const { return &m_RTTI; }
