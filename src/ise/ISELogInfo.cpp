#include "ISELogInfo.h"
#include <stdio.h>
#include <stdarg.h>

// Original group o-f322f9eddf22cd22fca0 (libISELib.a(ISELog.o)).

static volatile bool sbAssertPrintEnabled = true;

extern "C" {
void ISE_glViewport(int x, int y, int width, int height);
void ISE_glEnable(unsigned int cap);
void ISE_glDisable(unsigned int cap);
void ISE_glScissor(int x, int y, int width, int height);
void ISE_glBlendFunc(unsigned int sfactor, unsigned int dfactor);
void ISE_glBindBuffer(unsigned int target, unsigned int buffer);
}

void EnableAssertPrint(bool enable) {
    sbAssertPrintEnabled = enable;
}

void InitRenderStates() {
    ISE_glViewport(0, 0, 768, 1024);
    ISE_glEnable(0xc11);
    ISE_glScissor(0, 0, 768, 1024);
    ISE_glEnable(0xde1);
    ISE_glDisable(0xb71);
    ISE_glEnable(0xde1);
    ISE_glEnable(0xbe2);
    ISE_glBlendFunc(1, 0x303);
    ISE_glBindBuffer(0x8892, 0);
    ISE_glBindBuffer(0x8893, 0);
    ISE_glDisable(0xb44);
}

namespace ISE {

ISELogInfo *ISELogInfo::m_pInst = 0;

ISELogInfo::ISELogInfo() : m_enabled(true) {  }

void ISELogInfo::Log(const char *fmt, ...) {
    if (m_enabled) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

} // namespace ISE
