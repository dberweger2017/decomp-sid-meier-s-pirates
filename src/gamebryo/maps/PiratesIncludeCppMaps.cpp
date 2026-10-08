#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-0aaf8c6270adb89d12c9.
template void NiTMapBase<DFALL<int>, FStringA*, int>::ClearValue(NiTMapItem<FStringA*, int >*);
template unsigned int NiTMapBase<DFALL<int>, FStringA*, int>::KeyToHashIndex(FStringA*) const;
template bool NiTMapBase<DFALL<int>, FStringA*, int>::IsKeysEqual(FStringA*, FStringA*) const;
template void NiTMapBase<DFALL<int>, FStringA*, int>::SetValue(NiTMapItem<FStringA*, int >*, FStringA*, int);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::KeyToHashIndex(unsigned char) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::ClearValue(NiTMapItem<unsigned char, FStringA* >*);
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::IsKeysEqual(unsigned char, unsigned char) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned char, FStringA*>::SetValue(NiTMapItem<unsigned char, FStringA* >*, unsigned char, FStringA*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::ClearValue(NiTMapItem<NiNode*, Object3d* >*);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::KeyToHashIndex(NiNode*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::IsKeysEqual(NiNode*, NiNode*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, NiNode*, Object3d*>::SetValue(NiTMapItem<NiNode*, Object3d* >*, NiNode*, Object3d*);
