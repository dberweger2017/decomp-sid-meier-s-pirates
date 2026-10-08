#include <stdio.h>

// f-77dcd380797f3b107f6d — shipped printf forwarding; the original also treats its input as a format.
void OS_OutputDebugString(const char *message) { printf(message); }
