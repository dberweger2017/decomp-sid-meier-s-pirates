#include "FSharedSoundData.h"

// Original group: o-ac2c3029785c947aeb95 (FSharedSoundData.o).
// Missing methods remain undefined. No object-pool or vtable fallback is emitted.

unsigned char FSharedSoundData::GetBuffer(int index) { return m_bufferBytes[index]; }

unsigned int FSharedSoundData::GetBufferSize(int index) { return m_bufferSizes[index]; }

int FSharedSoundData::GetGlobalSoundFilenameIndex() { return m_globalSoundFilenameIndex; }

bool FSharedSoundData::IsStreamed() { return m_loadType == SoundLoadStreamed; }
