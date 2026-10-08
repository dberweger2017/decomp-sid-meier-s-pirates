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

void FSound3D::SetScript(const F3DSoundScriptData *script) { m_script = script; }

float FSound3D::GetTaperVolume() const { return m_taperVolume; }

void FSound3D::SetTaperVolume(float value) { m_taperVolume = value; }

bool FSound3D::SetVelocityMagnitude(float value) { m_velocityMagnitude = value; return true; }

void FSound3D::SetShortCircuitScriptField(int flags) { m_shortCircuitScriptFields |= flags; }

void FSound3D::ClearShortCircuitScriptField(int flags) { m_shortCircuitScriptFields &= ~flags; }

bool FSound3D::GetVolume(float &value) const { value = m_volume; return true; }

bool FSound3D::GetVelocityMagnitude(float &value) const { value = m_velocityMagnitude; return true; }
