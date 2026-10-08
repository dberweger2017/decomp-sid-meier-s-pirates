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
    ISENode * GetNodeByIndex(int index);

private:
    unsigned char m_unknown_00[244];
    ISENode ** m_nodes; // +0xf4
    unsigned char m_unknown_f8[12];
    unsigned int m_renderUnitCount; // +0x104
    unsigned int m_polyCount; // +0x108
};
} // namespace ISE
