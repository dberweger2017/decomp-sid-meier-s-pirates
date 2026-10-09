#pragma once

namespace ISE {

class ISEParticleEntity;

class ISEVisKey {
public:
    float m_time;
    unsigned char m_vis;

    ISEVisKey();
    static unsigned char GenInterp(float time, ISEVisKey* keys, unsigned int numKeys, unsigned int& lastIndex);
    void LoadBinary(ISEParticleEntity& entity);
};

} // namespace ISE
