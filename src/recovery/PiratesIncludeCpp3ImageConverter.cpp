// f-e3bdebc723ac03c22030 — load the observed image-converter singleton.
static void * volatile pirates_image_converter_global
    __asm__("__ZN16NiImageConverter14ms_spConverterE")
    __attribute__((aligned(16)));
extern "C" void * pirates_get_image_converter_e3bdebc723ac03c22030(void)
    __asm__("__ZN16NiImageConverter17GetImageConverterEv");
extern "C" void * pirates_get_image_converter_e3bdebc723ac03c22030(void) {
    return pirates_image_converter_global;
}
