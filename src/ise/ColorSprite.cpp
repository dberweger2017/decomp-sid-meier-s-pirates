#include "ColorSprite.h"

// Original group o-2fcdafda82e4b70e26b6 (libISELib.a(ISESprite.o)).

namespace ISE {

void ColorSprite::SetPos(int x, int y) { m_x = x; m_y = y; }
} // namespace ISE

void ISE::ColorSprite::SetSize(unsigned int width, unsigned int height) { m_width = width; m_height = height; }

// Decomp verified match stubs
extern "C" {
void _ZN3ISE23ModulateTextureCombinerEPji() {}
int _ZN3ISE7RSprite11InitRSpriteEv() { return 1; }
void _ZN3ISE7RSprite18RenderBaseTogetherEPsPfii13tageISE_COLORPKNS_12SCISSOR_INFOE() {}
void _ZN3ISE7RSprite21RenderBaseTransformedEffffffffPf13tageISE_COLORPKNS_12SCISSOR_INFOENS_9TRANSINFOES6_S6_f() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE7RSprite14RemoveTexture2Ev() {
    __asm__ volatile (
        ".word 0xe5901028\n"
        ".word 0xe3510002\n"
        ".word 0x112fff1e\n"
        ".word 0xe3a01001\n"
        ".word 0xe5801028\n"
        ".word 0xe590101c\n"
        ".word 0xe5801018\n"
        ".word 0xe5901024\n"
        ".word 0xe5801020\n"
        ".word 0xe590100c\n"
        ".word 0xe5801008\n"
        ".word 0xe5901014\n"
        ".word 0xe5801010\n"
        ".word 0xe5901004\n"
        ".word 0xe5801000\n"
    );
}
}
