#pragma once

// Partial nonpolymorphic layout established by getter offsets. Constructor,
// destructor, sound containers and their ownership remain unrecovered.
class FSoundScape {
public:
    FSoundScape();
    ~FSoundScape();
    bool IsInitialized() const;
    int GetScriptId();

private:
    unsigned char m_unknown_00[28];
    int m_scriptId;                         // +0x1c
    unsigned char m_unknown_20[10];
    bool m_initialized;                    // +0x2a
};
