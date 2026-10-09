static unsigned char pirates_ni_controller_manager_rtti[8]
    __asm__("__ZN19NiControllerManager6m_RTTIE") __attribute__((aligned(16)));
extern "C" const void * pirates_ni_controller_manager_get_rtti(void const *)
    __asm__("__ZNK19NiControllerManager7GetRTTIEv");
extern "C" const void * pirates_ni_controller_manager_get_rtti(void const *) {
    return pirates_ni_controller_manager_rtti;
}
