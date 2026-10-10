#include "ISEVisKey.h"

extern "C" void* memcpy(void*, const void*, unsigned int);

namespace ISE {

class StreamBuffer {
public:
    char* base;
    unsigned int size;
    char* ptr;
};

class ISEParticleEntity {
public:
    char pad[0x168];
    StreamBuffer* stream;
};

ISEVisKey::ISEVisKey() : m_time(0.0f), m_vis(0) {}

unsigned char ISEVisKey::GenInterp(float time, ISEVisKey* keys, unsigned int numKeys, unsigned int& lastIndex) {
    if (numKeys == 1) {
        return keys[0].m_vis;
    }
    unsigned int idx = lastIndex;
    if (keys[idx].m_time > time) {
        lastIndex = 0;
        idx = 0;
    }
    unsigned int maxKey = numKeys - 1;
    for (unsigned int i = idx + 1; i <= maxKey; ++i) {
        if (keys[i].m_time > time) {
            return keys[i - 1].m_vis;
        }
        lastIndex = i;
    }
    return keys[maxKey].m_vis;
}

void ISEVisKey::LoadBinary(ISEParticleEntity& entity) {
    StreamBuffer* sb = entity.stream;
    unsigned int avail = (sb->base + sb->size) - sb->ptr;
    unsigned int toRead = avail > 4 ? 4 : avail;
    if (toRead > 0) {
        memcpy(&m_time, sb->ptr, toRead);
        sb->ptr += toRead;
    }
    avail = (sb->base + sb->size) - sb->ptr;
    if (avail > 0) {
        unsigned char visibility = *reinterpret_cast<unsigned char*>(sb->ptr);
        m_vis = visibility != 0;
        sb->ptr += 1;
    }
}

} // namespace ISE
