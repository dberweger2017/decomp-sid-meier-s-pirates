#pragma once

#include "../../gamebryo/NiPoint3.h"

// Partial access path recovered from the original by-value position setter.
class FFontString {
public:
    void SetPosition(NiPoint3 position);

private:
    unsigned char m_unknown_00[0x34];
    void * m_textState; // +0x34
};
