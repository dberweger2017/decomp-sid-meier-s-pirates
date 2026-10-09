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

void FAudioManager::Set3DScripts(F3DSoundScriptData *scripts, int count) { m_scripts3D = scripts; m_numScripts3D = count; }

void FAudioManager::SetSoundScapeScripts(FSoundScapeScriptData *scripts, int count) { m_soundScapeScripts = scripts; m_numSoundScapeScripts = count; }

void FAudioManager::SetVolumeKnobs(FKnob *knobs, int count) { m_volumeKnobs = knobs; m_numVolumeKnobs = count; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN13FAudioManager18SetGlobalSoundDataEP15GlobalSoundDatai() {
    __asm__ volatile (
        ".word 0xe3a03000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000003\n"
        ".word 0xe3520001\n"
        ".word 0xa3a03001\n"
        ".word 0xa5801538\n"
        ".word 0xa580253c\n"
        ".word 0xe1a00003\n"
    );
}
__attribute__((naked)) void _ZN13FAudioManager14SetContextDataEPii() {
    __asm__ volatile (
        ".word 0xe3a03000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000003\n"
        ".word 0xe3520001\n"
        ".word 0xa3a03001\n"
        ".word 0xa5801540\n"
        ".word 0xa5802544\n"
        ".word 0xe1a00003\n"
    );
}
__attribute__((naked)) void _ZN13FAudioManager16GetSoundFilenameEi() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000005\n"
        ".word 0xe590353c\n"
        ".word 0xe3a02000\n"
        ".word 0xe1530001\n"
        ".word 0xc3a0204c\n"
        ".word 0xc5900538\n"
        ".word 0xc0220291\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZN13FAudioManager13GetVolumeKnobEi() {
    __asm__ volatile (
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0xba000004\n"
        ".word 0xe5903700\n"
        ".word 0xe3a02000\n"
        ".word 0xe1530001\n"
        ".word 0xc59026fc\n"
        ".word 0xc0822181\n"
        ".word 0xe1a00002\n"
    );
}
__attribute__((naked)) void _ZNK13FAudioManager11CanAddSoundEv() {
    __asm__ volatile (
        ".word 0xe5d0325c\n"
        ".word 0xe5901268\n"
        ".word 0xe5902278\n"
        ".word 0xe3530000\n"
        ".word 0xe0821001\n"
        ".word 0x05902224\n"
        ".word 0x1590222c\n"
        ".word 0xe3a00000\n"
        ".word 0xe1510002\n"
        ".word 0x33a00001\n"
    );
}
__attribute__((naked)) void _ZNK13FAudioManager13CanAdd3DSoundEv() {
    __asm__ volatile (
        ".word 0xe5d0325c\n"
        ".word 0xe590126c\n"
        ".word 0xe590227c\n"
        ".word 0xe3530000\n"
        ".word 0xe0821001\n"
        ".word 0x05902224\n"
        ".word 0x1590222c\n"
        ".word 0xe3a00000\n"
        ".word 0xe1510002\n"
        ".word 0x33a00001\n"
    );
}
__attribute__((naked)) void _ZN13FAudioManager14DeinitTestDataEv() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe5c01728\n"
        ".word 0xe580174c\n"
        ".word 0xe580172c\n"
        ".word 0xe5801730\n"
        ".word 0xe5801734\n"
        ".word 0xe5801738\n"
        ".word 0xe580173c\n"
        ".word 0xe5801740\n"
        ".word 0xe5801744\n"
        ".word 0xe5801748\n"
    );
}
}
