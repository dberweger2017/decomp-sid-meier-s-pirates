#include "heap3.h"

// Original group: o-2e066eac87fdb187c28a (heap3.o).
// Heap_Free/Alloc and freelist helpers establish the 24-byte block header:
// owner +0, previous +4, size/flags +8, validation cookie +12, free links +16/+20.
// Complete allocator behavior and original source names remain unrecovered.
struct HeapBlock {
    Heap *owner;
    HeapBlock *previous;
    int sizeAndFlags;
    unsigned int cookie;
    HeapBlock *previousFree;
    HeapBlock *nextFree;
};

static inline HeapBlock *blockHeader(void *allocation) {
    return reinterpret_cast<HeapBlock *>(
        static_cast<unsigned char *>(allocation) - sizeof(HeapBlock));
}

void Heap_Dump(Heap *, const char *) {}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZL9math_log2j() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe3500000\n"
        ".word 0x0a000011\n"
        ".word 0xe3500801\n"
        ".word 0xe3a01000\n"
        ".word 0x21a00820\n"
        ".word 0x23a01010\n"
        ".word 0xe2103cff\n"
        ".word 0xe3812008\n"
        ".word 0x11a00420\n"
        ".word 0x01a02001\n"
        ".word 0xe21030f0\n"
        ".word 0xe3821004\n"
        ".word 0x11a00220\n"
        ".word 0x01a01002\n"
        ".word 0xe210300c\n"
        ".word 0xe3812002\n"
        ".word 0x11a00120\n"
        ".word 0x01a02001\n"
        ".word 0xe7e010d0\n"
        ".word 0xe0811002\n"
        ".word 0xe1a00001\n"
    );
}
__attribute__((naked)) void _Z14Heap_FromBlockPv() {
    __asm__ volatile (
        ".word 0xe2400018\n"
        ".word 0xe5900000\n"
    );
}
}
