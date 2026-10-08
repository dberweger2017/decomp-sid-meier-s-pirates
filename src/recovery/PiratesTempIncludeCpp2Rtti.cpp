static unsigned char pirates_ni_object_rtti[8]
    __asm__("__ZN8NiObject6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_object_get_rtti(void const *)
    __asm__("__ZNK8NiObject7GetRTTIEv");
extern "C" const void * pirates_ni_object_get_rtti(void const *) {
    return pirates_ni_object_rtti;
}

static void * volatile pirates_ni_texture_rtti
    __asm__("__ZN9NiTexture6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_texture_get_rtti(void const *)
    __asm__("__ZNK9NiTexture7GetRTTIEv");
extern "C" const void * pirates_ni_texture_get_rtti(void const *) {
    return pirates_ni_texture_rtti;
}
