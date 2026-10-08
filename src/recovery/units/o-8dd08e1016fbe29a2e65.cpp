// Original compilation group o-8dd08e1016fbe29a2e65.
// f-bd9b762683c9bcf4713f — initialize the observed word at +0x54.
extern "C" void pirates_model_pod_ctor_bd9b762683c9bcf4713f(void *)
    __asm__("__ZN13CPVRTModelPODC1Ev");
extern "C" void pirates_model_pod_ctor_bd9b762683c9bcf4713f(void *self) {
    *reinterpret_cast<unsigned int *>(static_cast<unsigned char *>(self) + 0x54) = 0;
}
