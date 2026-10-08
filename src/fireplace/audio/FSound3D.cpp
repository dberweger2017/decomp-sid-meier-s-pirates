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

bool FSound3D::GetPitchChange(int &value) const { value = m_pitchChange; return true; }

bool FSound3D::GetOriginalPitch(unsigned long &value) const { value = m_originalPitch; return true; }

bool FSound3D::IsLooping() const { return m_loopCount != 0; }

bool FSound3D::GetShortCircuitScriptField(int flags) { return (m_shortCircuitScriptFields & flags) != 0; }

bool FSound3D::GetDistances(float &first, float &second) const {
    first = m_firstDistance;
    second = m_secondDistance;
    return true;
}

bool FSound3D::GetCone(int &firstAngle, int &secondAngle, float &gain) const {
    firstAngle = m_firstConeAngle;
    secondAngle = m_secondConeAngle;
    gain = m_coneGain;
    return true;
}

bool FSound3D::GetPosition(NiPoint3 &position) const {
    position = m_position;
    return true;
}
