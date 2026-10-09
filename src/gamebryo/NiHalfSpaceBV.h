#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Fail-fast bodies preserve original traps, not working implementations of
// the corresponding factories, animation or collision operations.
class NiHalfSpaceBV {
public:
    static bool HalfSpaceSphereTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool HalfSpaceBoxTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool HalfSpaceCapsuleTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool HalfSpaceHalfSpaceTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool HalfSpaceTriTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&);
    static bool HalfSpaceSphereFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool HalfSpaceBoxFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool HalfSpaceCapsuleFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool HalfSpaceHalfSpaceFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool HalfSpaceTriFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static NiBoundingVolume* CreateFromStream(NiStream&);
};
