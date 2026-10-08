#pragma once
#include "../recovery/ReleaseHookTypes.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiTriBasedGeomData {
public:
    void GetTriangleIndices(unsigned short, unsigned short&, unsigned short&, unsigned short&) const;
    void GetStripData(unsigned short&, unsigned short const*&, unsigned short const*&, unsigned int&) const;
    void SetActiveTriangleCount(unsigned short);
    unsigned short GetActiveTriangleCount() const;

private:
    unsigned char m_unknown_00[54];
    unsigned short m_activeCount; // +0x36
};
