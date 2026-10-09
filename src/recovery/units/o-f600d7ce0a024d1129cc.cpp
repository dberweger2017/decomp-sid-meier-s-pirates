// Original compilation group o-f600d7ce0a024d1129cc.

#include "../leaves/o-f600d7ce0a024d1129cc.cpp"

// f-7d8697698294eee94b5b — load the observed audio-manager global.
static void * volatile pirates_audio_manager_global __asm__("_gs_oAudioMgr")
    __attribute__((aligned(16)));
extern "C" void * pirates_get_audio_manager_7d8697698294eee94b5b(void)
    __asm__("__ZN16FAudioLibFactory11GetAudioMgrEv");
extern "C" void * pirates_get_audio_manager_7d8697698294eee94b5b(void) {
    return pirates_audio_manager_global;
}
