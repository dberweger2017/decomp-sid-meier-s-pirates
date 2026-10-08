#pragma once

namespace ISE {

struct ISELightVector4 {
    float x;
    float y;
    float z;
    float w;
};

// Partial layout recovered from the SetAmbientColor, SetDiffuseColor and
// SetLightPos stores. Other fields and the full type remain unknown.
class ISELight {
public:
    void SetAmbientColor(float red, float green, float blue);
    void SetDiffuseColor(float red, float green, float blue);
    void SetLightPos(float x, float y, float z);

private:
    unsigned char m_unknown_00[8];
    ISELightVector4 m_ambientColor; // +0x08; fourth component is cleared.
    ISELightVector4 m_diffuseColor; // +0x18; fourth component is cleared.
    unsigned char m_unknown_28[16];
    ISELightVector4 m_lightPosition; // +0x38; fourth component is set to 1.
};

} // namespace ISE
