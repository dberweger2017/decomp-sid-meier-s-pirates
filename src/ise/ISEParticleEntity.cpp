#include "ISEParticleEntity.h"

// Original group o-11c7f0138eabdedbcbfb (libISELib.a(ISEParticleEntity.o)).

namespace ISE {

unsigned int ISEParticleEntity::GetFileVersion() const { return m_fileVersion; }
} // namespace ISE

unsigned int ISE::ISEParticleEntity::GetVersion(unsigned int a, unsigned int b, unsigned int c, unsigned int d) { return (a << 24) | (b << 16) | (c << 8) | d; }
