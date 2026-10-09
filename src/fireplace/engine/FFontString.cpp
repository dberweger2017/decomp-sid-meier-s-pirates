#include "FFontString.h"

// Original group o-fec08f39a29b045b7d54 (FFontString.o).
void FFontString::SetPosition(NiPoint3 position) {
    *reinterpret_cast<NiPoint3 *>(static_cast<unsigned char *>(m_textState) + 0x5c) = position;
}
