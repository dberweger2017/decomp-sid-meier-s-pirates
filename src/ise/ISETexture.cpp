#include "ISETexture.h"

// Original group o-9b5e1bfcdd1c2e52775d (libISELib.a(ISETexture.o)).

namespace ISE {

bool ISETexture::IsAlphaEnabled() { return (m_flags >> 15) & 1; }
} // namespace ISE
