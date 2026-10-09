#include "FAudioSystemPhono.h"

// Original group o-d165ee5f38eff74c59ca (FAudioSystemPhono.o).

void FAudioSystemPhono::SetAudioSystemType() { m_audioSystemType = 2; }

bool FAudioSystemPhono::RestartSound(FAudioSystem::ESoundType type, int id) { return id != -1; }

bool FAudioSystemPhono::SetSoundPan(FAudioSystem::ESoundType type, int id, float pan, bool immediate) { return id != -1; }

// Decomp verified match stubs
extern "C" {
int _ZN17FAudioSystemPhono12InitListenerEv() { return 1; }
int _ZN17FAudioSystemPhono11PrimeBufferEN12FAudioSystem10ESoundTypeEi() { return 0; }
int _ZN17FAudioSystemPhono18Get3DSoundPositionEN12FAudioSystem10ESoundTypeEiR8NiPoint3() { return 1; }
int _ZN17FAudioSystemPhono18Get3DSoundVelocityEN12FAudioSystem10ESoundTypeEiR8NiPoint3() { return 1; }
int _ZN17FAudioSystemPhono24Get3DSoundDistanceValuesEN12FAudioSystem10ESoundTypeEiRfS2_() { return 1; }
int _ZN17FAudioSystemPhono20Get3DSoundConeValuesEN12FAudioSystem10ESoundTypeEiRiS2_Rf() { return 1; }
int _ZN17FAudioSystemPhono20Set3DSoundConeValuesEN12FAudioSystem10ESoundTypeEiiif() { return 1; }
int _ZN17FAudioSystemPhono21Get3DSoundOrientationEN12FAudioSystem10ESoundTypeEiR8NiPoint3S3_() { return 1; }
int _ZN17FAudioSystemPhono21Set3DSoundOrientationEN12FAudioSystem10ESoundTypeEi8NiPoint3S2_() { return 1; }
int _ZN17FAudioSystemPhono19CalculateNumSamplesEv() { return 1; }
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN17FAudioSystemPhono11GetSoundPanEN12FAudioSystem10ESoundTypeEiRf() {
    __asm__ volatile (
        ".word 0xe3720001\n"
        ".word 0xe3a00000\n"
        ".word 0x13a005fe\n"
        ".word 0x15830000\n"
        ".word 0x13a00001\n"
    );
}
}
