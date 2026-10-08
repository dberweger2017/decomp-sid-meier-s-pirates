#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Fail-fast bodies preserve original traps, not working implementations of
// the corresponding factories, animation or collision operations.
class NiBoxBV {
public:
    static bool BoxSphereTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool BoxBoxTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool BoxCapsuleTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool BoxTriTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&);
    static bool BoxSphereFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool BoxBoxFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool BoxCapsuleFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool BoxTriFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static NiBoundingVolume* CreateFromStream(NiStream&);
};
