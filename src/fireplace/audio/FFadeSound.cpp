#include "FFadeSound.h"

EFadeState FFadeSound::GetState() { return m_state; }
void FFadeSound::SetState(EFadeState state) { m_state = state; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}
