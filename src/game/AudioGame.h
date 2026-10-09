#pragma once

#include <vector>

enum E2DAS_Type {
    E2DAS_TYPE_NONE = 0
};

enum ESoundContextType {
    SOUND_CONTEXT_NONE = -1
};

enum EAudioGameMode {
    AUDIO_GAME_MODE_NONE = 0
};

class FISound;
class FISound3D;
class FSoundScape;
struct FSoundtrackGroupNode;
class PCamera_xia;
class NiAVObject;

template <typename T>
class NiPointer {
public:
    NiPointer() : m_pObject(0) {}
    NiPointer(T* pObject) : m_pObject(pObject) {}
    T* m_pObject;
};

struct NiPoint3 {
    float x, y, z;
};

class FStringA {
public:
    FStringA();
    ~FStringA();
private:
    char* m_data;
};

class AudioQueue {
public:
    bool IsEmpty();
};

class AudioGame {
public:
    AudioGame();
    ~AudioGame();

    static AudioGame* GetAudioGame();

    int GetAudioCueTime(int index);
    ESoundContextType GetCurrentSoundContext();
    void DeinitAudioCues();
    int GetPlayTime();
    void InitAudioCues(E2DAS_Type type, int count);
    FStringA Get3DAudioDriverDesc();
    void Set3DAudioDriverDesc(FStringA desc);
    bool InitAudioManager(bool flag);
    bool DeinitAudioManager();
    bool QueueIsEmpty();
    float GetVolumeKnob(int knob);
    void Deinit();
    bool Init();

    void UnloadAll(int flag, ESoundContextType* context);

private:
    int m_unknown0; // offset 0x00
    AudioQueue m_audioQueue; // offset 0x04
    char m_pad1[0x2744]; // offset 0x08 - 0x274c
    std::vector<ESoundContextType> m_soundContexts; // offset 0x2750
    std::vector<EAudioGameMode> m_gameModes;       // offset 0x275c
    char m_pad2[0x10];
    int m_audioCueCount;                           // offset 0x2774
    void* m_audioCues;                             // offset 0x2778
    char m_pad3[0x04];
};

typedef char static_assert_audiogame_size[sizeof(AudioGame) == 10112 ? 1 : -1];
