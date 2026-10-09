#pragma once
#include "../recovery/ReleaseHookTypes.h"



// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiParticlesData {
public:
    void CalculateNormals();
    unsigned short GetActiveVertexCount() const;

private:
    unsigned char m_unknown_00[60];
    unsigned short m_activeCount; // +0x3c
};
