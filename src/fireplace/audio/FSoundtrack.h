#pragma once

class FSoundtrackGroupNode;

// Partial layout for direct comparisons; do not instantiate. Complete size,
// hierarchy, virtual slots and remaining methods are unrecovered. Unencoded
// return types, pointees, signedness and field names remain hypotheses.
class FSoundtrack {
public:
    void SetSongGroup(const FSoundtrackGroupNode *group, int index);
    void SetTaperVolume(float volume);

private:
    unsigned char m_unknown_00[24];
    const FSoundtrackGroupNode * m_songGroup; // +0x18
    int m_songGroupIndex; // +0x1c
    unsigned char m_unknown_20[12];
    float m_taperVolume; // +0x2c
};
