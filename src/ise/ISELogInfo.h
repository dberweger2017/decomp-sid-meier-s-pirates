#pragma once

#include <stdarg.h>

namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISELogInfo {
public:
    ISELogInfo();
    void Log(const char *fmt, ...);

    static ISELogInfo *m_pInst;

private:
    bool m_enabled; // +0x00
};

} // namespace ISE

void EnableAssertPrint(bool enable);
void PrintChar(char c, int x, int y);
void InitRenderStates();
void AssertPrintAndDeadLoop(const char *msg, const char *file, int line);
void InitForScreenPrinting();
