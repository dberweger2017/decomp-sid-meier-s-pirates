#pragma once

// Original TracingList enumerators are unknown.
enum TracingList { TracingListUnknown = -1 };

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class NPCInfoUIScene {
public:
    void updateNPCInfo(TracingList list);

private:
    unsigned char m_unknown_00[76];
    TracingList m_tracingList; // +0x4c
};
