#pragma once

// Partial original heap ABI. Heap_IsMyBlock reads bounds at offsets 0 and 4.
// Only those bounds are declared; allocator/locking state remains unrecovered.
struct Heap {
    const unsigned char *begin;
    const unsigned char *end;
};

void Heap_Dump(Heap *heap, const char *label);
Heap *Heap_FromBlock(void *allocation);
bool Heap_IsMyBlock(Heap *heap, const void *allocation);
bool Heap_IsValidPointer(void *allocation);
