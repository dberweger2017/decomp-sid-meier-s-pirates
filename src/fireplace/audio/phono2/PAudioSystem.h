#pragma once

// Declaration subset for named calls observed in FPhono.o. Missing Phono2
// implementation stays in its original object group. The mangled symbols prove
// argument types; unencoded return types are provisional, guided by call use.
// No instance layout or vtable is reconstructed by this declaration subset.
namespace Phono2 {
class PAudioSystem {
public:
    static PAudioSystem *getSingletonPtr();
    void StopStream(unsigned int stream);
    void SetStreamPlaybackPosition(unsigned int stream, unsigned int position);
    unsigned int GetStreamPlaybackTotal(unsigned int stream);
    unsigned int GetStreamPlaybackPosition(unsigned int stream);
    void SetSamplePlaybackPosition(unsigned int sample, unsigned int position);
    unsigned int GetSamplePlaybackPosition(unsigned int sample);
    void SetChannelVolume(unsigned int channel, float volume);
    float GetChannelVolume(unsigned int channel);
    bool IsStreamLooping(unsigned int stream);
    bool IsSampleLooping(unsigned int sample);
    void SetChannelPitch(unsigned int channel, float pitch);
    float GetChannelPlaybackRate(unsigned int channel);
    void StopSample(unsigned int sample);
    bool IsStreamPlaying(unsigned int stream);
    bool IsSamplePlaying(unsigned int sample);
    void EnableDoppler(float factor, float parameter);
    void Update(float elapsed);
    void ReGenSources();
};
}
