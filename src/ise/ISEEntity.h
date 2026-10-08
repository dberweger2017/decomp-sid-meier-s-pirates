#pragma once


namespace ISE {
class ISENode;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEEntity {
public:
    unsigned int GetRenderUnitNum();
    unsigned int GetPolyNum();
    void * GetRenderUnit(int index);
    ISENode * GetNodeByIndex(int index);

private:
    unsigned char m_unknown_00[244];
    ISENode ** m_nodes; // +0xf4
    unsigned char m_unknown_f8[4];
    void * m_renderUnits; // +0xfc
    unsigned char m_unknown_100[4];
    unsigned int m_renderUnitCount; // +0x104
    unsigned int m_polyCount; // +0x108
};

// Partial view of the material path observed in this object's getter.
class ISEEntityRenderUnit {
public:
    void * GetMaterial();

private:
    unsigned char m_unknown_00[20];
    void * m_materialOwner; // +0x14
};
} // namespace ISE
