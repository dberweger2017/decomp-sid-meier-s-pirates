// Original compilation group o-daf3c29e4a4853ee61a4.

#include "../leaves/o-daf3c29e4a4853ee61a4.cpp"

// f-64817babecd5ef822a05 — initialize the manager's first state word.
extern "C" void pirates_faudio_mem_mgr_ctor(void *self)
    __asm__("__ZN12FAudioMemMgrC1Ev");
extern "C" void pirates_faudio_mem_mgr_ctor(void *self) {
    *reinterpret_cast<unsigned int *>(self) = 0;
}
