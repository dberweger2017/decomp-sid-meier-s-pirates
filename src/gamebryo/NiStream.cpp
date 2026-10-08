#include "NiStream.h"

// Original group o-6031b6c2de40a8188a31 (TempIncludeCpp4.o).

NiObjectGroup * NiStream::GetGroupFromID(unsigned int id) { return m_groups[id]; }

NiTexturePalette * NiStream::GetTexturePalette() const { return m_texturePalette; }
