#include "PVRTResourceFile.h"

// Original group: o-4268c237f42393c8bef0 (libOGLES2Tools.a(PVRTResourceFile.o)).
// Add only individually verified definitions. This scaffold earns no progress.

bool CPVRTResourceFile::IsOpen() const { return m_open; }

unsigned long CPVRTResourceFile::Size() const { return m_size; }

const char *CPVRTResourceFile::StringPtr() const { return m_data; }

// f-43c051ba9889a12d9302 — forward static cleanup to the registered target.
extern "C" void pirates_static_cleanup_target(void)
    __asm__("__ZN21CPVRTMemoryFileSystem7CAtExitD1Ev");
extern "C" void pirates_static_cleanup_43c051ba9889a12d9302(void)
    __asm__("___tcf_1");
extern "C" void pirates_static_cleanup_43c051ba9889a12d9302(void) {
    pirates_static_cleanup_target();
}
