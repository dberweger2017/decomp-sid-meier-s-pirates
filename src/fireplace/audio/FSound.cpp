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

const F2DSoundScriptData *FSound::GetScript() { return m_script; }

void FSound::SetScript(const F2DSoundScriptData *script) { m_script = script; }

float FSound::GetTaperVolume() const { return m_taperVolume; }

void FSound::SetTaperVolume(float value) { m_taperVolume = value; }

void FSound::SetShortCircuitScriptField(int flags) { m_shortCircuitScriptFields |= flags; }

void FSound::ClearShortCircuitScriptField(int flags) { m_shortCircuitScriptFields &= ~flags; }

bool FSound::GetVolume(float &value) const { value = m_volume; return true; }

bool FSound::GetPan(float &value) const { value = m_pan; return true; }

bool FSound::GetPitchChange(int &value) const { value = m_pitchChange; return true; }

bool FSound::GetOriginalPitch(unsigned long &value) const { value = m_originalPitch; return true; }

#include "../../recovery/abi/o-22e9019c653c60382da6.cpp"
