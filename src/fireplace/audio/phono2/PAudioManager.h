#pragma once


namespace Phono2 {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class PAudioManager {
public:
    void Shutdown();

private:
    unsigned int * m_begin; // +0x00
    unsigned int * m_end; // +0x04
};
} // namespace Phono2
