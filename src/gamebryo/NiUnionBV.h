#pragma once
#include "../recovery/SmallFunctionTypes.h"

// Partial interface for observed shipped tiny bodies; do not instantiate.
// Complete inheritance/layout/virtual slots and unencoded results are unknown.
// Fail-fast bodies preserve original traps, not working implementations of
// the corresponding factories, animation or collision operations.
class NiUnionBV {
public:
    static bool UnionOtherTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool UnionUnionTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&);
    static bool UnionTriTestIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&);
    static bool UnionOtherFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool UnionUnionFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiBoundingVolume const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static bool UnionTriFindIntersect(float, NiBoundingVolume const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, NiPoint3 const&, float&, NiPoint3&, bool, NiPoint3&, NiPoint3&);
    static NiBoundingVolume* CreateFromStream(NiStream&);
};
