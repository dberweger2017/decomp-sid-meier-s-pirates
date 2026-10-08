#include "PVRTString.h"

// Original group: o-1f9e1182f8fd5b86d0bb (libOGLES2Tools.a(PVRTString.o)).
// Add only individually verified definitions. This scaffold earns no progress.

const char *CPVRTString::c_str() const { return m_buffer; }

unsigned long CPVRTString::length() const { return m_length; }
