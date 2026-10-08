#include "FAudioMgr.h"

// Original group: o-b9afd2a4c28d8e343b6a (FAudioMgr.o).
// Missing methods remain undefined; no artificial stubs or complete type claim.

float FAudioManager::GetTime() const { return m_time; }

bool FAudioManager::IsInitialized() const { return m_initialized; }

bool FAudioManager::IsPaused() { return m_paused; }

GlobalSoundData *FAudioManager::GetGlobalSoundData() { return m_globalSoundData; }

int *FAudioManager::GetContextDataBits() { return m_contextDataBits; }

F2DSoundScriptData *FAudioManager::Get2DScripts() { return m_scripts2D; }

F3DSoundScriptData *FAudioManager::Get3DScripts() { return m_scripts3D; }

FSoundScapeScriptData *FAudioManager::GetSoundScapeScripts() { return m_soundScapeScripts; }

int FAudioManager::GetNum2DScripts() { return m_numScripts2D; }

FKnob *FAudioManager::GetVolumeKnobs() { return m_volumeKnobs; }

float FAudioManager::GetDopplerFactor() { return m_dopplerFactor; }

float FAudioManager::GetDistanceFactor() { return m_distanceFactor; }

void FAudioManager::Set2DScripts(F2DSoundScriptData *scripts, int count) { m_scripts2D = scripts; m_numScripts2D = count; }
