#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-7d25f6180d9844d7c020.
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::KeyToHashIndex(NiObject*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::IsKeysEqual(NiObject*, NiObject*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::ClearValue(NiTMapItem<NiObject*, NiObject* >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>::SetValue(NiTMapItem<NiObject*, NiObject* >*, NiObject*, NiObject*);
