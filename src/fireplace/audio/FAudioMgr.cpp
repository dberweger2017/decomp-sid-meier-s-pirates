#include "FAudioMgr.h"

// Original group: o-b9afd2a4c28d8e343b6a (FAudioMgr.o).
// Missing methods remain undefined; no artificial stubs or complete type claim.

float FAudioManager::GetTime() const { return m_time; }

bool FAudioManager::IsInitialized() const { return m_initialized; }

bool FAudioManager::IsPaused() { return m_paused; }

GlobalSoundData *FAudioManager::GetGlobalSoundData() { return m_globalSoundData; }

int *FAudioManager::GetContextDataBits() { return m_contextDataBits; }

F2DSoundScriptData *FAudioManager::Get2DScripts() { return m_scripts2D; }
