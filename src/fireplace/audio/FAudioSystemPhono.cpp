#include "FAudioSystemPhono.h"

// Original group o-d165ee5f38eff74c59ca (FAudioSystemPhono.o).

void FAudioSystemPhono::SetAudioSystemType() { m_audioSystemType = 2; }

bool FAudioSystemPhono::RestartSound(FAudioSystem::ESoundType type, int id) { return id != -1; }

bool FAudioSystemPhono::SetSoundPan(FAudioSystem::ESoundType type, int id, float pan, bool immediate) { return id != -1; }

#include "FAudioSystemPhonoSmall_FAudioSystemPhono.cpp"

#include "../../recovery/abi/o-d165ee5f38eff74c59ca.cpp"
