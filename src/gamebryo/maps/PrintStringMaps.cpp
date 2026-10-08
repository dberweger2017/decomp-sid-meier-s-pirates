#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-d2c464150a675c39482a.
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, PrintedText*>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, PrintedText*>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, PrintedText*>::ClearValue(NiTMapItem<char const*, PrintedText* >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, PrintedText*>::SetValue(NiTMapItem<char const*, PrintedText* >*, char const*, PrintedText*);

#include "../../recovery/abi/o-d2c464150a675c39482a.cpp"

#include "../../recovery/leaves/o-d2c464150a675c39482a.cpp"
