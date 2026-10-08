#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-a70c52d41737dcc20b28.
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::Animation*>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::Animation*>::IsKeysEqual(unsigned int, unsigned int) const;
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::LayerGroup*>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::LayerGroup*>::IsKeysEqual(unsigned int, unsigned int) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::Animation*>::ClearValue(NiTMapItem<unsigned int, NiKFMTool::Animation* >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::Animation*>::SetValue(NiTMapItem<unsigned int, NiKFMTool::Animation* >*, unsigned int, NiKFMTool::Animation*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::LayerGroup*>::ClearValue(NiTMapItem<unsigned int, NiKFMTool::LayerGroup* >*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, unsigned int, NiKFMTool::LayerGroup*>::SetValue(NiTMapItem<unsigned int, NiKFMTool::LayerGroup* >*, unsigned int, NiKFMTool::LayerGroup*);
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned int>::ClearValue(NiTMapItem<char const*, unsigned int >*);
template unsigned int NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned int>::KeyToHashIndex(char const*) const;
template bool NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned int>::IsKeysEqual(char const*, char const*) const;
template void NiTMapBase<NiTPointerAllocator<unsigned int>, char const*, unsigned int>::SetValue(NiTMapItem<char const*, unsigned int >*, char const*, unsigned int);
