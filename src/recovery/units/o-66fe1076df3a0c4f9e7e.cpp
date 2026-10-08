// Original compilation group o-66fe1076df3a0c4f9e7e.

#include "../leaves/o-66fe1076df3a0c4f9e7e.cpp"

// f-b71caead8ef23ec27bb5 — return the observed audio-game object address.
static unsigned char pirates_audio_game_instance[10112]
    __asm__("__ZL13gs_oAudioGame") __attribute__((aligned(16)));
extern "C" const void * pirates_audio_game_get_instance(void const *)
    __asm__("__ZN9AudioGame12GetAudioGameEv");
extern "C" const void * pirates_audio_game_get_instance(void const *) {
    return pirates_audio_game_instance;
}
