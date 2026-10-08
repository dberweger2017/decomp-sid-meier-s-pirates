#pragma once

// Partial ABI views for observed map helper bodies. Do not instantiate maps or
// allocate items: complete hierarchy, allocator state, virtual slots, lifetime
// operations and layout beyond accessed fields are unrecovered. ClearValue is
// the observed no-op hook, not proof that referenced objects need no cleanup.
// Key comparison is pointer/value identity, including char const* keys.
// Hashing converts the ARMv7 key value to 32 bits before unsigned remainder.
#include "NiKFMTool.h"
class NiTexture;
class NiControllerSequence;
class AnimationInfo;
class FTerrainCell;
class FxWidget;
class Object3d;
class NiObject;
class PrintedText;
class FStringA;
class NiNode;
template <class T> class DFALL;
template <class T> class NiTPointerAllocator;
template <class T> class NiPointer;

template <class Key, class Value> struct NiTMapItem {
    NiTMapItem *m_next; // +0x00; next-link usage needs further recovery
    Key m_key;         // +0x04 for selected ARMv7 key types
    Value m_value;     // +0x08, including alignment after an 8-bit key
};

template <class Allocator, class Key, class Value> class NiTMapBase {
public:
    unsigned int KeyToHashIndex(Key key) const;
    bool IsKeysEqual(Key left, Key right) const;
    void ClearValue(NiTMapItem<Key, Value> *item);
    void SetValue(NiTMapItem<Key, Value> *item, Key key, Value value);
private:
    unsigned char m_unknown_00[4];
    unsigned int m_bucketCount; // +0x04
};

template <class A, class K, class V>
unsigned int NiTMapBase<A, K, V>::KeyToHashIndex(K key) const {
    return (unsigned int)key % m_bucketCount;
}

template <class A, class K, class V>
bool NiTMapBase<A, K, V>::IsKeysEqual(K left, K right) const {
    return left == right;
}

template <class A, class K, class V>
void NiTMapBase<A, K, V>::ClearValue(NiTMapItem<K, V> *) {}

template <class A, class K, class V>
void NiTMapBase<A, K, V>::SetValue(NiTMapItem<K, V> *item, K key, V value) {
    item->m_key = key;
    item->m_value = value;
}
