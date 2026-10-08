#pragma once

class PCamera_xia;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FListener {
public:
    void Set3DObject(PCamera_xia *object);
    PCamera_xia * Get3DObject();

private:
    unsigned char m_unknown_00[40];
    PCamera_xia * m_object3D; // +0x28
};
