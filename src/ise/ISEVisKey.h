#pragma once

namespace ISE {

class ISEParticleEntity;

class ISEVisKey {
public:
    float m_time;
    bool m_vis;

    ISEVisKey();
    static bool GenInterp(float time, ISEVisKey* keys, unsigned int numKeys, unsigned int& lastIndex);
    void LoadBinary(ISEParticleEntity& entity);
};

} // namespace ISE
