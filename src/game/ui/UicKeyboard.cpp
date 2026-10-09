#include "UicKeyboard.h"

// Recovered bodies from PiratesIncludeCpp2.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

void UicKeyboard::Reset() { m_active = false; }

void UicKeyboard::SetTextFont(Font *font) {
    *reinterpret_cast<void **>(static_cast<unsigned char *>(m_fontOwner) + 0x7c) = font;
    m_font = font;
}
