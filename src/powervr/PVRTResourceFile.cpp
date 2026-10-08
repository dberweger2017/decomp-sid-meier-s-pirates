#include "PVRTResourceFile.h"

// Original group: o-4268c237f42393c8bef0 (libOGLES2Tools.a(PVRTResourceFile.o)).
// Add only individually verified definitions. This scaffold earns no progress.

bool CPVRTResourceFile::IsOpen() const { return m_open; }

unsigned long CPVRTResourceFile::Size() const { return m_size; }

const char *CPVRTResourceFile::StringPtr() const { return m_data; }
