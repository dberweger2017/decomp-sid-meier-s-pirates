#include "ReleaseHookTypes.h"

int OS_InterlockedDecrement(long*) { return 0; }
int OS_GetCurrentThreadId() { return 0; }
int OS_VkKeyScan(char) { return 0; }
int OS_GetAsyncKeyState(int) { return 0; }
int OS_GetTickCount() { return 0; }
