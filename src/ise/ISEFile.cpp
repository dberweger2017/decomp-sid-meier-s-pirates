#include "ISEFile.h"

// Original group o-4ecf6698b47bb4fae018 (libISELib.a(ISEFile.o)).

namespace ISE {

unsigned int ISEFile::Size() const { return m_size; }

unsigned char * ISEFile::BufferPtr() const { return m_buffer; }
} // namespace ISE

#include "../recovery/abi/o-4ecf6698b47bb4fae018.cpp"
