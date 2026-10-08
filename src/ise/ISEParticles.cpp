#include "ISEParticles.h"

// Original group o-22e518abce9579acfc03 (libISELib.a(ISEParticles.o)).

namespace ISE {

void ISEParticles::SetModelData(ISEParticleGeometryData *data) { if (data) m_modelData = data; }

void * ISEParticles::GetMaterial() {
    return reinterpret_cast<unsigned char *>(m_materialOwner) + 8;
}
} // namespace ISE

#include "ISEParticlesSmall_libISELib_a_ISEParticles_.cpp"

#include "../recovery/abi/o-22e518abce9579acfc03.cpp"

#include "../recovery/leaves/o-22e518abce9579acfc03.cpp"
