#pragma once

class NiCamera;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NiAccumulator {
public:
    void FinishAccumulating();

    bool StartAccumulating(NiCamera *camera);

private:
    unsigned char m_unknown_00[12];
    NiCamera * m_camera; // +0x0c
};
