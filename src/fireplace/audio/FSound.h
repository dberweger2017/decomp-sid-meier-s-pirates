#pragma once

struct F2DSoundScriptData;

// Partial ARMv7 object layout, inferred from the original accessor offsets.
// Unknown byte ranges preserve offsets without inventing their types/behavior.
// The complete type, constructor, destructor, ownership and base hierarchy are
// not recovered. Do not instantiate this partial declaration as a game object.
class FSound {
public:
    virtual ~FSound();
    int GetSampleId();
    int GetStreamId();
    unsigned int GetGlobalSoundFilenameIndex();
    bool IsStreaming();
    bool IsLooping() const;
    bool IsInitialized() const;
    bool IsLoaded() const;
    bool IsPaused() const;
    bool GetToBeDestroyed();
    bool GetIsMusic() const;
    void SetIsMusic(bool value);
    const F2DSoundScriptData *GetScript();
    void SetScript(const F2DSoundScriptData *script);
    float GetTaperVolume() const;
    void SetTaperVolume(float value);
    bool GetVolume(float &value) const;
    bool GetPan(float &value) const;
    bool GetPitchChange(int &value) const;
    bool GetOriginalPitch(unsigned long &value) const;
    void SetShortCircuitScriptField(int flags);
    void ClearShortCircuitScriptField(int flags);
    bool GetShortCircuitScriptField(int flags);

private:
    unsigned char m_unknown_04[8];
    int m_sampleId;                            // +0x0c
    int m_streamId;                            // +0x10
    unsigned int m_globalSoundFilenameIndex;   // +0x14
    bool m_streaming;                         // +0x18
    unsigned char m_unknown_19[3];
    int m_looping;                            // +0x1c
    unsigned char m_unknown_20[4];
    float m_volume;                           // +0x24
    float m_pan;                              // +0x28
    int m_pitchChange;                        // +0x2c
    unsigned long m_originalPitch;            // +0x30
    float m_taperVolume;                      // +0x34
    unsigned char m_unknown_38[32];
    bool m_toBeDestroyed;                     // +0x58
    bool m_isMusic;                           // +0x59
    unsigned char m_unknown_5a[2];
    const F2DSoundScriptData *m_script;        // +0x5c
    int m_shortCircuitScriptFields;           // +0x60
    unsigned char m_unknown_64[20];
    bool m_initialized;                       // +0x78
    bool m_loaded;                            // +0x79
    unsigned char m_unknown_7a;
    bool m_paused;                            // +0x7b
};
