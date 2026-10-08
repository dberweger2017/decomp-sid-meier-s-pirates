#include <CoreFoundation/CFDate.h>

// f-ade82c6549be070ab325 — SDK-defined double result, independently named import.
namespace ISE { double GetCurrentTimeD() { return CFAbsoluteTimeGetCurrent(); } }
