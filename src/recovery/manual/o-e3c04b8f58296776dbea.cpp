// Hand-recovered bodies for original group o-e3c04b8f58296776dbea.
// f-a77090de2965eec5d727 — adjusted subobject getter.
// The original thunk adds 0x7c to the incoming subobject view.
extern "C" void * pirates_adjusted_getter_a77090de2965eec5d727(void * a0)
    __asm__("__ZThn180_N3ISE15ISEEditableMesh11GetMaterialEv");
extern "C" void * pirates_adjusted_getter_a77090de2965eec5d727(void * a0) {
    return static_cast<unsigned char *>(a0) + 0x7c;
}

