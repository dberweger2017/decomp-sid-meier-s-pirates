#pragma once
#include "../powervr/PVRTString.h"

namespace ISE {
class ISEFile;

template <typename T>
class ISEManagerBase {
public:
    T *AddObject(const CPVRTString &name, const CPVRTString &group);
};

extern ISEManagerBase<ISEFile> gISEFileManager;
} // namespace ISE
