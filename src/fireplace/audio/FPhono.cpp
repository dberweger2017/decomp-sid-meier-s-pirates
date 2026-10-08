#include "FPhono.h"
#include "phono2/PAudioSystem.h"

// Original group: o-78c5a9284f33537c011b (FPhono.o).
// Only named source calls are reconstructed. Phono2 targets stay unresolved at
// replacement link time until recovered; comparison resolves their original
// symbol addresses and verifies the encoded call targets.

unsigned int FPhono::GetPlaybackTotal_Stream(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetStreamPlaybackTotal(stream);
}

unsigned int FPhono::GetPlaybackPosition_Stream(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetStreamPlaybackPosition(stream);
}

unsigned int FPhono::GetPlaybackPosition_Sample(int sample) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetSamplePlaybackPosition(sample);
}

float FPhono::GetVolume_Sound(int sound) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetChannelVolume(sound);
}

bool FPhono::GetLooping_Stream(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->IsStreamLooping(stream);
}

bool FPhono::GetLooping_Sample(int sample) {
    return Phono2::PAudioSystem::getSingletonPtr()->IsSampleLooping(sample);
}

float FPhono::GetPlaybackRate_Sound(int sound) {
    return Phono2::PAudioSystem::getSingletonPtr()->GetChannelPlaybackRate(sound);
}

void FPhono::Stop_Sample(int sample) {
    Phono2::PAudioSystem::getSingletonPtr()->StopSample(sample);
}

bool FPhono::IsStreamPlaying(int stream) {
    return Phono2::PAudioSystem::getSingletonPtr()->IsStreamPlaying(stream);
}

bool FPhono::IsSamplePlaying(int sample) {
    return Phono2::PAudioSystem::getSingletonPtr()->IsSamplePlaying(sample);
}

void FPhono::Update(float elapsed) {
    Phono2::PAudioSystem::getSingletonPtr()->Update(elapsed);
}

void FPhono::Shutdown() {
    Phono2::PAudioSystem::getSingletonPtr()->ReGenSources();
}

void FPhono::SetPlaybackPosition_Stream(int stream, unsigned int position) {
    Phono2::PAudioSystem::getSingletonPtr()->SetStreamPlaybackPosition(stream, position);
}
