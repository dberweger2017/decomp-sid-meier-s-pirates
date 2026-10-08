#include "FSoundScape.h"

// Original group: o-71a79dd832a29b589321 (FSoundScape.o).
// Missing constructor/destructor/container behavior remains undefined.

bool FSoundScape::IsInitialized() const { return m_initialized; }

int FSoundScape::GetScriptId() { return m_scriptId; }
