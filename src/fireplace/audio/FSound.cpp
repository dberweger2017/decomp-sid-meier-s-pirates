#include "FSound.h"

// Original group: o-22e9019c653c60382da6 (FSound.o).
// Missing methods remain undefined; do not provide artificial stubs.

int FSound::GetSampleId() { return m_sampleId; }

int FSound::GetStreamId() { return m_streamId; }

unsigned int FSound::GetGlobalSoundFilenameIndex() { return m_globalSoundFilenameIndex; }

bool FSound::IsStreaming() { return m_streaming; }

bool FSound::IsInitialized() const { return m_initialized; }

bool FSound::IsLoaded() const { return m_loaded; }

bool FSound::IsPaused() const { return m_paused; }

bool FSound::GetToBeDestroyed() { return m_toBeDestroyed; }

bool FSound::GetIsMusic() const { return m_isMusic; }

void FSound::SetIsMusic(bool value) { m_isMusic = value; }
