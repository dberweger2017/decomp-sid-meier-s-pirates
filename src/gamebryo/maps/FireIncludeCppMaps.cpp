#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-692e206382b21043c376.
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiPointer<NiTexture> >::KeyToHashIndex(char const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiPointer<NiTexture> >::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, NiPointer<NiTexture> >::ClearValue(NiTMapItem<char const*, NiPointer<NiTexture> >*);
template unsigned int NiTMapBase<DFALL<NiPointer<NiControllerSequence> >, char const*, NiPointer<NiControllerSequence> >::KeyToHashIndex(char const*) const;
template unsigned int NiTMapBase<DFALL<NiKFMTool*>, char const*, NiKFMTool*>::KeyToHashIndex(char const*) const;
template unsigned int NiTMapBase<DFALL<AnimationInfo*>, unsigned int, AnimationInfo*>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<DFALL<AnimationInfo*>, unsigned int, AnimationInfo*>::IsKeysEqual(unsigned int, unsigned int) const;
template void NiTMapBase<DFALL<AnimationInfo*>, unsigned int, AnimationInfo*>::ClearValue(NiTMapItem<unsigned int, AnimationInfo* >*);
template bool NiTMapBase<DFALL<NiPointer<NiControllerSequence> >, char const*, NiPointer<NiControllerSequence> >::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<DFALL<NiPointer<NiControllerSequence> >, char const*, NiPointer<NiControllerSequence> >::ClearValue(NiTMapItem<char const*, NiPointer<NiControllerSequence> >*);
template bool NiTMapBase<DFALL<NiKFMTool*>, char const*, NiKFMTool*>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<DFALL<NiKFMTool*>, char const*, NiKFMTool*>::ClearValue(NiTMapItem<char const*, NiKFMTool* >*);
template void NiTMapBase<DFALL<NiKFMTool*>, char const*, NiKFMTool*>::SetValue(NiTMapItem<char const*, NiKFMTool* >*, char const*, NiKFMTool*);
template void NiTMapBase<DFALL<AnimationInfo*>, unsigned int, AnimationInfo*>::SetValue(NiTMapItem<unsigned int, AnimationInfo* >*, unsigned int, AnimationInfo*);
template void NiTMapBase<DFALL<unsigned char*>, unsigned int, unsigned char*>::ClearValue(NiTMapItem<unsigned int, unsigned char* >*);
template unsigned int NiTMapBase<DFALL<unsigned char*>, unsigned int, unsigned char*>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<DFALL<unsigned char*>, unsigned int, unsigned char*>::IsKeysEqual(unsigned int, unsigned int) const;
template void NiTMapBase<DFALL<unsigned char*>, unsigned int, unsigned char*>::SetValue(NiTMapItem<unsigned int, unsigned char* >*, unsigned int, unsigned char*);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, FTerrainCell*>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, FTerrainCell*>::IsKeysEqual(unsigned int, unsigned int) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, FTerrainCell*>::ClearValue(NiTMapItem<unsigned int, FTerrainCell* >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, FTerrainCell*>::SetValue(NiTMapItem<unsigned int, FTerrainCell* >*, unsigned int, FTerrainCell*);
template unsigned int NiTMapBase<DFALL<FxWidget*>, char const*, FxWidget*>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<DFALL<FxWidget*>, char const*, FxWidget*>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<DFALL<FxWidget*>, char const*, FxWidget*>::ClearValue(NiTMapItem<char const*, FxWidget* >*);
template void NiTMapBase<DFALL<FxWidget*>, char const*, FxWidget*>::SetValue(NiTMapItem<char const*, FxWidget* >*, char const*, FxWidget*);
