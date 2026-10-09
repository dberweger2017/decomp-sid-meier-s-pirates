#pragma once


namespace ISE {
class ISEEntity;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISENode {
public:
    ISENode * SetParent(ISENode *parent);
    void * GetName();
    void SetEntity(ISEEntity *entity);

private:
    unsigned char m_unknown_00[736];
    ISENode * m_parent; // +0x2e0
    unsigned char m_unknown_2e4[12];
    ISEEntity * m_entity; // +0x2f0
};
} // namespace ISE
