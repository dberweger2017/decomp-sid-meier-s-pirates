#include "FSound3D.h"

// Original group: o-b2988598bdd8217340d1 (FSound3D.o).
// Missing methods remain undefined; do not provide artificial stubs.

int FSound3D::GetSampleId() { return m_sampleId; }

int FSound3D::GetStreamId() { return m_streamId; }

unsigned int FSound3D::GetGlobalSoundFilenameIndex() { return m_globalSoundFilenameIndex; }

bool FSound3D::IsStreaming() { return m_streaming; }

bool FSound3D::IsInitialized() const { return m_initialized; }

bool FSound3D::IsLoaded() const { return m_loaded; }

bool FSound3D::IsPaused() const { return m_paused; }

bool FSound3D::GetToBeDestroyed() { return m_toBeDestroyed; }

bool FSound3D::GetIsMusic() const { return m_isMusic; }

void FSound3D::SetIsMusic(bool value) { m_isMusic = value; }

const F3DSoundScriptData *FSound3D::GetScript() { return m_script; }
