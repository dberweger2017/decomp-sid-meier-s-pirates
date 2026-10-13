#include "ISEVisKey.h"
#include <cstring>
#include <cstddef>

namespace ISE {

class StreamBuffer {
public:
    char* base; // +0x00
    char* ptr; // +0x04
    size_t size; // +0x08

    int Read(void* destination, int requested) {
        int avail = (base + size) - ptr;
        int toRead = avail > requested ? requested : avail;
        if (toRead != 0) {
            if (destination) {
                memcpy(destination, ptr, toRead);
            }
            ptr += toRead;
        }
        return toRead;
    }
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
    entity.stream->Read(&m_time, 4);
    unsigned char visibility;
    m_vis = entity.stream->Read(&visibility, 1) ? visibility != 0 : 0;
}

} // namespace ISE
