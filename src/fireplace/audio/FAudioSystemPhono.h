#pragma once
#include "../../recovery/SmallFunctionTypes.h"
#include "FAudioSystem.h"
#include "../../gamebryo/NiPoint3.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FAudioSystemPhono {
public:
    bool InitListener();
    bool PrimeBuffer(FAudioSystem::ESoundType, int);
    bool Get3DSoundPosition(FAudioSystem::ESoundType, int, NiPoint3&);
    bool Get3DSoundVelocity(FAudioSystem::ESoundType, int, NiPoint3&);
    bool Get3DSoundDistanceValues(FAudioSystem::ESoundType, int, float&, float&);
    bool Get3DSoundConeValues(FAudioSystem::ESoundType, int, int&, int&, float&);
    bool Set3DSoundConeValues(FAudioSystem::ESoundType, int, int, int, float);
    bool Get3DSoundOrientation(FAudioSystem::ESoundType, int, NiPoint3&, NiPoint3&);
    unsigned int CalculateNumSamples();
    void SetAudioSystemType();

    bool RestartSound(FAudioSystem::ESoundType type, int id);

    bool SetSoundPan(FAudioSystem::ESoundType type, int id, float pan, bool immediate);

private:
    unsigned char m_unknown_00[48];
    int m_audioSystemType; // +0x30
};
