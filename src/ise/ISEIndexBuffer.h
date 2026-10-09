#pragma once

namespace ISE {

class IndexBuffer {
public:
    IndexBuffer(int count);
    ~IndexBuffer();

    int GetCount() const { return m_count; }
    unsigned short* GetBuffer() { return m_buffer; }

private:
    int m_count;
    int m_offset;
    unsigned short* m_buffer;
    int m_capacity;
};

} // namespace ISE
