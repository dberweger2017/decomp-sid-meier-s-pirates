// Isolated source probe for f-5a027fbd3240a6a0e288. No instruction payload.
struct NiRTTI;
class NiObject {
public:
    static const NiRTTI m_RTTI;
    const NiRTTI *GetRTTI() const;
};
const NiRTTI *NiObject::GetRTTI() const { return &m_RTTI; }
