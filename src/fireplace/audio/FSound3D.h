#pragma once

struct F3DSoundScriptData;

// Partial ARMv7 ABI established by accessor loads/stores. Unknown fields and
// original base classes remain unrecovered. This is not an instantiable complete
// game type; constructor, destructor and ownership behavior remain missing.
class FSound3D {
public:
    virtual ~FSound3D();
    int GetSampleId();
    int GetStreamId();
    unsigned int GetGlobalSoundFilenameIndex();
    bool IsStreaming();
    bool IsInitialized() const;
    bool IsLoaded() const;
    bool IsPaused() const;
    bool GetToBeDestroyed();
    bool GetIsMusic() const;
    void SetIsMusic(bool value);
    const F3DSoundScriptData *GetScript();
    void SetScript(const F3DSoundScriptData *script);
    float GetTaperVolume() const;
    void SetTaperVolume(float value);
    bool SetVelocityMagnitude(float value);
    bool GetVelocityMagnitude(float &value) const;
    bool GetVolume(float &value) const;
    bool GetPitchChange(int &value) const;
    bool GetOriginalPitch(unsigned long &value) const;
    void SetShortCircuitScriptField(int flags);
    void ClearShortCircuitScriptField(int flags);

private:
    unsigned char m_unknown_04[8];
    int m_sampleId;                            // +0x0c
    int m_streamId;                            // +0x10
    unsigned int m_globalSoundFilenameIndex;   // +0x14
    bool m_streaming;                          // +0x18
    unsigned char m_unknown_19[47];
    float m_velocityMagnitude;                 // +0x48
    unsigned char m_unknown_4c[44];
    float m_volume;                            // +0x78
    int m_pitchChange;                         // +0x7c
    unsigned long m_originalPitch;             // +0x80
    float m_taperVolume;                       // +0x84
    unsigned char m_unknown_88[32];
    bool m_toBeDestroyed;                      // +0xa8
    bool m_isMusic;                            // +0xa9
    unsigned char m_unknown_aa[6];
    const F3DSoundScriptData *m_script;         // +0xb0
    int m_shortCircuitScriptFields;            // +0xb4
    unsigned char m_unknown_b8[12];
    bool m_initialized;                        // +0xc4
    bool m_loaded;                             // +0xc5
    unsigned char m_unknown_c6;
    bool m_paused;                             // +0xc7
};
