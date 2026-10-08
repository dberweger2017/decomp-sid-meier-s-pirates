#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Fail-fast bodies preserve original traps, not working implementations of
// the corresponding factories, animation or collision operations.
class NiCapsuleBV {
public:
    static bool CapsuleSphereTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool CapsuleCapsuleTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool CapsuleTriTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&);
    static bool CapsuleSphereFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool CapsuleCapsuleFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool CapsuleTriFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static NiBoundingVolume* CreateFromStream(NiStream&);
};
