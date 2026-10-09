#include "ISEUtility.h"

// Original group o-9d6495d5b7de5d9d762a (libISELib.a(ISEUtility.o)).

namespace ISE {

CPVRTString ISEGetFileNameWithOutExtension(const CPVRTString& path) {
    CPVRTString ret(path, 0, CPVRTString::npos);
    CPVRTString ext = PVRTStringGetFileExtension(path);
    if (ext.length() != 0) {
        ret.erase(ret.length() - ext.length(), CPVRTString::npos);
    }
    return ret;
}

} // namespace ISE
