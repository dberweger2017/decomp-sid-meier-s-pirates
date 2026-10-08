#pragma once

struct FSharedSoundDataInit;

// Only the values observed in Clear/IsStreamed are named. Other original
// enumerators and source spellings are unknown; mangled Load proves this type.
enum ESoundLoadType { SoundLoadUnknown = -1, SoundLoadStreamed = 2 };

// Original RTTI records FISharedSoundData as the base. The concrete vtable lists
// these slots in this order, with no virtual destructor slot. Status-returning
// methods use provisional bool types; mangled names omit return types. Buffer's
// byte return follows the observed ldrb, not a guessed buffer pointer API.
// No complete hierarchy, ownership policy, constructor or runtime claim.
class FISharedSoundData {
public:
    virtual bool Init(const FSharedSoundDataInit &initialization) = 0;
    virtual void Destroy() = 0;
    virtual bool Load(int filenameIndex, ESoundLoadType loadType, int flags, char *name) = 0;
    virtual bool Unload() = 0;
    virtual unsigned char GetBuffer(int index) = 0;
    virtual unsigned int GetBufferSize(int index) = 0;
    virtual unsigned int GetTotalBufferSize() = 0;
    virtual int GetGlobalSoundFilenameIndex() = 0;
    virtual bool IsStreamed() = 0;
    virtual ESoundLoadType GetLoadType() = 0;
    virtual int GetNumInstancesInUse() = 0;
    virtual bool IncNumInstancesInUse() = 0;
    virtual bool DecNumInstancesInUse() = 0;
    virtual void Clear() = 0;
};

class FSharedSoundData : public FISharedSoundData {
public:
    FSharedSoundData();
    ~FSharedSoundData();
    virtual bool Init(const FSharedSoundDataInit &initialization);
    virtual void Destroy();
    virtual bool Load(int filenameIndex, ESoundLoadType loadType, int flags, char *name);
    virtual bool Unload();
    virtual unsigned char GetBuffer(int index);
    virtual unsigned int GetBufferSize(int index);
    virtual unsigned int GetTotalBufferSize();
    virtual int GetGlobalSoundFilenameIndex();
    virtual bool IsStreamed();
    virtual ESoundLoadType GetLoadType();
    virtual int GetNumInstancesInUse();
    virtual bool IncNumInstancesInUse();
    virtual bool DecNumInstancesInUse();
    virtual void Clear();

private:
    unsigned char m_unknown_04[4];
    // Observed byte-addressed view; the complete initialization payload is unknown.
    unsigned char m_bufferBytes[160];           // +0x008
    unsigned int m_bufferSizes[32];            // +0x0a8
    unsigned char m_unknown_128[4];
    int m_globalSoundFilenameIndex;            // +0x12c
    int m_numInstancesInUse;                   // +0x130
    bool m_unknownFlag;                        // +0x134
    unsigned char m_unknown_135[3];
    ESoundLoadType m_loadType;                  // +0x138
    bool m_initialized;                        // +0x13c
};
