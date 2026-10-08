#pragma once

struct GlobalSoundData;
struct F2DSoundScriptData;
struct F3DSoundScriptData;
struct FSoundScapeScriptData;
struct FKnob;

// Partial ARMv7 ABI. Getter/store offsets establish the fields below; mangled
// setter signatures establish pointer types. Update uses floating-point math on
// time. Unknown fields and the complete original hierarchy remain unrecovered.
// Constructor/destructor and object ownership are not supplied by this scaffold.
class FAudioManager {
public:
    virtual ~FAudioManager();
    float GetTime() const;
    bool IsInitialized() const;
    bool IsPaused();
    GlobalSoundData *GetGlobalSoundData();
    int *GetContextDataBits();
    F2DSoundScriptData *Get2DScripts();
    F3DSoundScriptData *Get3DScripts();
    FSoundScapeScriptData *GetSoundScapeScripts();
    int GetNum2DScripts();
    void Set2DScripts(F2DSoundScriptData *scripts, int count);
    void Set3DScripts(F3DSoundScriptData *scripts, int count);
    void SetSoundScapeScripts(FSoundScapeScriptData *scripts, int count);
    FKnob *GetVolumeKnobs();
    void SetVolumeKnobs(FKnob *knobs, int count);
    float GetDopplerFactor();
    float GetDistanceFactor();
    bool SetGlobalSoundData(GlobalSoundData *data, int count);
    bool SetContextData(int *data, int count);

private:
    unsigned char m_unknown_04[4];
    bool m_initialized;                          // +0x008
    unsigned char m_unknown_009[519];
    float m_time;                                // +0x210
    unsigned char m_unknown_214[52];
    float m_dopplerFactor;                       // +0x248
    float m_distanceFactor;                      // +0x24c
    unsigned char m_unknown_250[8];
    bool m_paused;                               // +0x258
    unsigned char m_unknown_259[735];
    GlobalSoundData *m_globalSoundData;            // +0x538
    int m_numGlobalSounds;                       // +0x53c
    int *m_contextDataBits;                      // +0x540
    int m_numContextData;                        // +0x544
    unsigned char m_unknown_548[408];
    F2DSoundScriptData *m_scripts2D;              // +0x6e0
    int m_numScripts2D;                          // +0x6e4
    F3DSoundScriptData *m_scripts3D;              // +0x6e8
    int m_numScripts3D;                          // +0x6ec
    FSoundScapeScriptData *m_soundScapeScripts;    // +0x6f0
    int m_numSoundScapeScripts;                   // +0x6f4
    unsigned char m_unknown_6f8[4];
    FKnob *m_volumeKnobs;                        // +0x6fc
    int m_numVolumeKnobs;                        // +0x700
};
