#pragma once


namespace ISE {

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class ISEFile {
public:
    unsigned int Size() const;
    unsigned char * BufferPtr() const;

private:
    unsigned char m_unknown_00[52];
    unsigned int m_size; // +0x34
    unsigned char * m_buffer; // +0x38
};
} // namespace ISE
