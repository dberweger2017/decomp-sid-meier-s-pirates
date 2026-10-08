#pragma once
#include "FAudioSystem.h"


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FAudioSystemPhono {
public:
    void SetAudioSystemType();

    bool RestartSound(FAudioSystem::ESoundType type, int id);

    bool SetSoundPan(FAudioSystem::ESoundType type, int id, float pan, bool immediate);

private:
    unsigned char m_unknown_00[48];
    int m_audioSystemType; // +0x30
};
