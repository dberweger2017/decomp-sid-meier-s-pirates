#include "ISEIndexBuffer.h"

// Original group o-6fc673363c5278413290 (libISELib.a(ISEIndexBuffer.o)).

namespace ISE {

IndexBuffer::IndexBuffer(int count)
    : m_count(count)
    , m_offset(0)
    , m_buffer(0)
    , m_capacity(count) {
    if (count >= 1) {
        m_buffer = new unsigned short[count];
    }
}

IndexBuffer::~IndexBuffer() {
    delete[] m_buffer;
}

} // namespace ISE
