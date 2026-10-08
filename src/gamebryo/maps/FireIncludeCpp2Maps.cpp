#include "../NiTMapBase.h"

// Explicit helper instantiations retain original object group o-fb173e414b8edec47528.
template unsigned int NiTMapBase<DFALL<unsigned int>, unsigned int, unsigned int>::KeyToHashIndex(unsigned int) const;
template bool NiTMapBase<DFALL<unsigned int>, unsigned int, unsigned int>::IsKeysEqual(unsigned int, unsigned int) const;
template void NiTMapBase<DFALL<unsigned int>, unsigned int, unsigned int>::ClearValue(NiTMapItem<unsigned int, unsigned int >*);
template void NiTMapBase<DFALL<unsigned int>, unsigned int, unsigned int>::SetValue(NiTMapItem<unsigned int, unsigned int >*, unsigned int, unsigned int);
