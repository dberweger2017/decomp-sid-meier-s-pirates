#include "ISELight.h"

// Original group o-6ffa9d64951f7100f022 (libISELib.a(ISELight.o)).
namespace ISE {

void ISELight::SetAmbientColor(float red, float green, float blue) {
    m_ambientColor = (ISELightVector4){red, green, blue, 0.0f};
}

void ISELight::SetDiffuseColor(float red, float green, float blue) {
    m_diffuseColor = (ISELightVector4){red, green, blue, 0.0f};
}

void ISELight::SetLightPos(float x, float y, float z) {
    m_lightPosition = (ISELightVector4){x, y, z, 1.0f};
}

} // namespace ISE
