#pragma once


// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FSoundQueue {
public:
    bool QueueIsActive();

private:
    unsigned char m_unknown_00[60];
    bool m_active; // +0x3c
};
