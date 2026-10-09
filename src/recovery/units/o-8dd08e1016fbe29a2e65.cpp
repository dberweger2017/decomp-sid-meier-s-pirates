// Original compilation group o-8dd08e1016fbe29a2e65.
// f-bd9b762683c9bcf4713f — initialize the observed word at +0x54.
extern "C" void pirates_model_pod_ctor_bd9b762683c9bcf4713f(void *)
    __asm__("__ZN13CPVRTModelPODC1Ev");
extern "C" void pirates_model_pod_ctor_bd9b762683c9bcf4713f(void *self) {
    *reinterpret_cast<unsigned int *>(static_cast<unsigned char *>(self) + 0x54) = 0;
}

// f-240dfc2f9bdc44845c75 — the complete destructor forwards to Destroy.
extern "C" void pirates_model_pod_destructor_240dfc2f9bdc44845c75(void *)
    __asm__("__ZN13CPVRTModelPODD1Ev") __attribute__((naked));
extern "C" void pirates_model_pod_destructor_240dfc2f9bdc44845c75(void *self) {
    __asm__ volatile("b __ZN13CPVRTModelPOD7DestroyEv");
}
