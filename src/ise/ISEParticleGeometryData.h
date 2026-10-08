#pragma once
#include "../recovery/ReleaseHookTypes.h"



namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEParticleGeometryData {
public:
    void SetActiveVertexCount(unsigned short);
    unsigned short GetActiveVertexCount() const;

private:
    unsigned char m_unknown_00[4];
    unsigned short m_activeVertexCount; // +0x04
};
} // namespace ISE
