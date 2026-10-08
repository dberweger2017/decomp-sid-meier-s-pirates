#include "FFileIO.h"

// Recovered bodies from FireIncludeCpp.o. Empty callbacks describe
// this shipped release build; they are not substitute implementations.

unsigned int FFileIO::GetLength() const { return m_length; }

bool FFileIO::IsOpen() const { return m_file != 0; }
