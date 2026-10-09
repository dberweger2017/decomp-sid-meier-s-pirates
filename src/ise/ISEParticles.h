#pragma once
#include "../recovery/SmallFunctionTypes.h"



namespace ISE {
class ISEParticleGeometryData;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEParticles {
public:
    unsigned int GetVertexNum();
    void * GetMaterial();
    void SetModelData(ISEParticleGeometryData *data);

private:
    unsigned char m_unknown_00[28];
    ISEParticleGeometryData * m_modelData; // +0x1c
    unsigned char m_unknown_20[36];
    void * m_materialOwner; // +0x44, observed by GetMaterial.
};
} // namespace ISE
