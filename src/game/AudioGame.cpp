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
    return *(int*)((char*)FAudioLibFactory::GetAudioMgr() + 0x750);
}

ESoundContextType AudioGame::GetCurrentSoundContext() {
    if (!m_soundContexts.empty()) {
        return m_soundContexts.back();
    }
    return (ESoundContextType)-1;
}

int AudioGame::GetAudioCueTime(int index) {
    if ((unsigned int)index < (unsigned int)m_audioCueCount) {
        char* cues = (char*)m_audioCues;
        return *(int*)(cues + index * 24 + 4);
    }
    return -1;
}

bool AudioGame::QueueIsEmpty() {
    return m_audioQueue.IsEmpty();
}

float AudioGame::GetVolumeKnob(int knob) {
    return FAudioLibFactory::GetAudioMgr()->GetVolumeKnob(knob);
}

void AudioGame::DeinitAudioManager() {
    UnloadAll(0, 0);
    FAudioLibFactory::GetAudioMgr()->Deinit();
}
