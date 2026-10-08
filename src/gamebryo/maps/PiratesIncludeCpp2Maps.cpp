#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-2d59da5048d3db995d0f.
template unsigned int NiTMapBase<DFALL<int>, char const*, int>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<DFALL<int>, char const*, int>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<DFALL<int>, char const*, int>::ClearValue(NiTMapItem<char const*, int >*);
template void NiTMapBase<DFALL<int>, char const*, int>::SetValue(NiTMapItem<char const*, int >*, char const*, int);
template unsigned int NiTMapBase<DFALL<NiPointer<Object3d> >, char const*, NiPointer<Object3d> >::KeyToHashIndex(char const*) const;
template bool NiTMapBase<DFALL<NiPointer<Object3d> >, char const*, NiPointer<Object3d> >::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<DFALL<NiPointer<Object3d> >, char const*, NiPointer<Object3d> >::ClearValue(NiTMapItem<char const*, NiPointer<Object3d> >*);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, Object3d*, int>::KeyToHashIndex(Object3d*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, Object3d*, int>::IsKeysEqual(Object3d*, Object3d*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, Object3d*, int>::ClearValue(NiTMapItem<Object3d*, int >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, Object3d*, int>::SetValue(NiTMapItem<Object3d*, int >*, Object3d*, int);
