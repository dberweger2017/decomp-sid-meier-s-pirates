#include "AudioGame.h"

// Original group o-66fe1076df3a0c4f9e7e (AudioGame.o).

class FAudioMgr {
public:
    virtual ~FAudioMgr();
    virtual void Deinit();
    float GetVolumeKnob(int knob);
};

class FAudioLibFactory {
public:
    static FAudioMgr* GetAudioMgr();
};

static AudioGame gs_oAudioGame;

AudioGame* AudioGame::GetAudioGame() {
    return &gs_oAudioGame;
}

int AudioGame::GetPlayTime() {
    return *((int*)((char*)FAudioLibFactory::GetAudioMgr() + 0x750));
}

ESoundContextType AudioGame::GetCurrentSoundContext() {
    if (m_soundContexts.empty()) {
        return SOUND_CONTEXT_NONE;
    }
    return m_soundContexts.back();
}

int AudioGame::GetAudioCueTime(int index) {
    if (index >= 0 && index < m_audioCueCount && m_audioCues != 0) {
        return *((int*)((char*)m_audioCues + index * 24 + 4));
    }
    return 0;
}

bool AudioGame::QueueIsEmpty() {
    return m_audioQueue.IsEmpty();
}

float AudioGame::GetVolumeKnob(int knob) {
    return FAudioLibFactory::GetAudioMgr()->GetVolumeKnob(knob);
}

bool AudioGame::DeinitAudioManager() {
    UnloadAll(0, 0);
    FAudioLibFactory::GetAudioMgr()->Deinit();
    return true;
}
