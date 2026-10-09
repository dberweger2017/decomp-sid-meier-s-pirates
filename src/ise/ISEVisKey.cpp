#include "ISEVisKey.h"
#include <string.h>

// Original group o-d0728268a4b50f53168e (libISELib.a(ISEVisKey.o)).

namespace ISE {

struct StreamBuffer {
    char* base;
    char* ptr;
    size_t size;
};

class ISEParticleEntity {
public:
    char pad[0x168];
    StreamBuffer* stream;
};

ISEVisKey::ISEVisKey() : m_time(0.0f), m_vis(false) {}

bool ISEVisKey::GenInterp(float time, ISEVisKey* keys, unsigned int numKeys, unsigned int& lastIndex) {
    if (numKeys == 1) {
        return keys[0].m_vis;
    }
    unsigned int idx = lastIndex;
    if (keys[idx].m_time > time) {
        lastIndex = 0;
        idx = 0;
    }
    for (unsigned int i = idx + 1; i <= numKeys - 1; ++i) {
        if (keys[i].m_time > time) {
            return keys[i - 1].m_vis;
        }
        lastIndex = i;
    }
    return keys[numKeys - 1].m_vis;
}

void ISEVisKey::LoadBinary(ISEParticleEntity& entity) {
    StreamBuffer* sb = entity.stream;
    size_t avail = (sb->base + sb->size) - sb->ptr;
    size_t toRead = avail > 4 ? 4 : avail;
    if (toRead > 0) {
        memcpy(&m_time, sb->ptr, toRead);
        sb->ptr += toRead;
    }

    sb = entity.stream;
    avail = (sb->base + sb->size) - sb->ptr;
    toRead = avail > 1 ? 1 : avail;
    char b = 0;
    if (toRead > 0) {
        memcpy(&b, sb->ptr, toRead);
        sb->ptr += toRead;
    }
    m_vis = (b != 0);
}

} // namespace ISE
