#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-6031b6c2de40a8188a31.
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject const*, unsigned int>::KeyToHashIndex(NiObject const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject const*, unsigned int>::IsKeysEqual(NiObject const*, NiObject const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject const*, unsigned int>::ClearValue(NiTMapItem<NiObject const*, unsigned int >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject const*, unsigned int>::SetValue(NiTMapItem<NiObject const*, unsigned int >*, NiObject const*, unsigned int);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiObject* (*)()>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiObject* (*)()>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiObject* (*)()>::ClearValue(NiTMapItem<char const*, NiObject* (*)() >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiObject* (*)()>::SetValue(NiTMapItem<char const*, NiObject* (*)() >*, char const*, NiObject* (*)());
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned short>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned short>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned short>::ClearValue(NiTMapItem<char const*, unsigned short >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned short>::SetValue(NiTMapItem<char const*, unsigned short >*, char const*, unsigned short);
