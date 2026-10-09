#include "ISEParticles.h"

// Original group o-22e518abce9579acfc03 (libISELib.a(ISEParticles.o)).

namespace ISE {

void ISEParticles::SetModelData(ISEParticleGeometryData *data) { if (data) m_modelData = data; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
int _ZThn12_N3ISE12ISEParticles12GetVertexNumEv() { return 0; }
int _ZN3ISE12ISEParticles12GetVertexNumEv() { return 0; }
int _ZThn4_N3ISE12ISEParticles14DrawInDrawListEPj() { return 0; }
int _ZN3ISE12ISEParticles14DrawInDrawListEPj() { return 0; }
int _ZThn12_N3ISE12ISEParticles17IsDrawListEnabledEv() { return 1; }
int _ZN3ISE12ISEParticles17IsDrawListEnabledEv() { return 1; }
}
