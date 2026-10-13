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

Heap *Heap_FromBlock(void *allocation) {
    HeapBlock *header = blockHeader(allocation);
    return header->owner;
}

void Heap_Dump(Heap *, const char *) {}

// Decomp leaf match stubs
extern "C" {


}
