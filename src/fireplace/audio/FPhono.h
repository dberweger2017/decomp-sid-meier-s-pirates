#pragma once

// Partial original static facade. Stream/sample IDs occupy r0 at call sites,
// proving these wrappers have no implicit this argument. Missing methods and
// complete original declarations remain unrecovered. Return types that are not
// encoded in symbols remain provisional; this header does not claim runtime ABI.
class FPhono {
public:
    static void CloseStream(int stream);
    static void SetPlaybackPosition_Stream(int stream, unsigned int position);
    static unsigned int GetPlaybackTotal_Stream(int stream);
    static unsigned int GetPlaybackPosition_Stream(int stream);
    static void SetPlaybackPosition_Sample(int sample, unsigned int position);
    static unsigned int GetPlaybackPosition_Sample(int sample);
    static void SetVolume_Sound(int sound, float volume);
    static float GetVolume_Sound(int sound);
    static bool GetLooping_Stream(int stream);
    static bool GetLooping_Sample(int sample);
    static void SetPlaybackRate_Sound(int sound, float pitch);
    static float GetPlaybackRate_Sound(int sound);
    static void Stop_Sample(int sample);
    static bool IsStreamPlaying(int stream);
    static bool IsSamplePlaying(int sample);
    static void SetDopplerFactor(float factor);
    static void Update(float elapsed);
    static void Shutdown();
};
