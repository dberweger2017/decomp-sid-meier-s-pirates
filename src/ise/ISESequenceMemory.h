#pragma once
#include <stddef.h>
#include <stdlib.h>

// Partial allocator ABI observed in the sequence readers. Enum names are
// provisional; original mangling establishes the types and instruction immediates.
enum AllocMethod { SequenceArrayAllocation = 1 };
class MemoryManager {
public:
    enum EHeapType { SequenceHeap = 0, SequenceForeignThreadHeap = 2 };
    static MemoryManager *GetSingletonInstance() throw();
    void *AllocWithHeap(EHeapType heap, unsigned int bytes);
    bool IsMyBlock(const void *block) throw();
    void Free(void *block, AllocMethod method) throw();
    unsigned char m_unknown000[0x264];
    EHeapType m_selectedHeap;                 // +0x264
    unsigned char m_unknown268[12];
    unsigned char m_overrideHeap;             // +0x274
};
bool IsThreadSet(MemoryManager::EHeapType heap);
bool IsThisThread(MemoryManager::EHeapType heap);

inline __attribute__((always_inline)) MemoryManager::EHeapType SequenceAllocationHeap() {
    MemoryManager *manager = MemoryManager::GetSingletonInstance();
    if (manager->m_overrideHeap) return manager->m_selectedHeap;
    if (IsThreadSet(MemoryManager::SequenceHeap) && !IsThisThread(MemoryManager::SequenceHeap))
        return MemoryManager::SequenceForeignThreadHeap;
    return MemoryManager::SequenceHeap;
}
inline __attribute__((always_inline)) void *operator new(size_t bytes) {
    MemoryManager *manager = MemoryManager::GetSingletonInstance();
    return manager->AllocWithHeap(SequenceAllocationHeap(), bytes);
}
inline __attribute__((always_inline)) void *operator new[](size_t bytes) {
    MemoryManager *manager = MemoryManager::GetSingletonInstance();
    return manager->AllocWithHeap(SequenceAllocationHeap(), bytes);
}
inline __attribute__((always_inline)) void operator delete[](void *block) throw() {
    if (!block) return;
    if (!MemoryManager::GetSingletonInstance()->IsMyBlock(block)) free(block);
    else MemoryManager::GetSingletonInstance()->Free(block, SequenceArrayAllocation);
}
