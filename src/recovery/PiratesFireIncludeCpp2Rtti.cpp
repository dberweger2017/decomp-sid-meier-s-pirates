static unsigned char pirates_fx_widget_rtti[8]
    __asm__("__ZN8FxWidget6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_fx_widget_get_rtti(void const *)
    __asm__("__ZNK8FxWidget7GetRTTIEv");
extern "C" const void * pirates_fx_widget_get_rtti(void const *) {
    return pirates_fx_widget_rtti;
}
