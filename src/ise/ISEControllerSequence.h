#pragma once
#include "../powervr/PVRTMathTypes.h"

namespace ISE {
// Partial target ABI from GetNodeMatrix and the sequence array destructor.
// Full channel layouts and construction behavior are being recovered separately.
class KeyframeController {
public:
    KeyframeController();
    virtual ~KeyframeController() throw();
    void GetScaleMatrix(float time, PVRTMATRIXf &matrix);
    void GetRotationMatrix(float time, PVRTMATRIXf &matrix);
    void GetTranslationMatrix(float time, PVRTMATRIXf &matrix);
private:
    friend class ControllerSequence;
    unsigned int m_unknown04;
    unsigned int m_metadata[4];         // +0x08..+0x17
    unsigned char m_unrecovered18[12];  // +0x18..+0x23
    template<class T> struct Channel { int count; float *times; T *values; };
    Channel<PVRTVECTOR3f> m_translation;    // +0x24
    Channel<PVRTQUATERNIONf> m_rotation;    // +0x30
    Channel<PVRTVECTOR3f> m_scale;          // +0x3c
    Channel<unsigned char> m_visibility;   // +0x48, introduced in version 2
    unsigned char m_unrecovered54[16];  // +0x54..+0x63
};

class ControllerSequence {
public:
    ~ControllerSequence();
    void GetNodeMatrix(int node, PVRTMATRIXf &matrix);
    static ControllerSequence *CreateSequenceFromMemory(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer1(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer2(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer3(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer4(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer5(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromMemoryVer6(const char *memory, int size);
    static ControllerSequence *CreateSequenceFromFile(const char *path);

    struct Node { char *name; int parent; };
private:
    template<int Version> static inline __attribute__((always_inline)) ControllerSequence *ReadSequence(const char *memory);
    char *m_name;                       // +0x00
    unsigned int m_unknown04;           // +0x04
    unsigned int m_unknown08;           // +0x08
    int m_propertyCount;                // +0x0c
    Node *m_properties;                 // +0x10, 8-byte records
    int m_nodeCount;                    // +0x14
    Node *m_nodes;                       // +0x18
    KeyframeController *m_keyframes;    // +0x1c, 100-byte objects
    int m_floatCount;                   // +0x20
    char **m_floatNames;                // +0x24
    class SequenceFloatController *m_floats; // +0x28, 56-byte objects
    int m_secondaryFloatCount;          // +0x2c
    char **m_secondaryFloatNames;       // +0x30
    SequenceFloatController *m_secondaryFloats; // +0x34
    unsigned char m_unrecovered38[12];  // +0x38..+0x43
    float m_time;                       // +0x44
};
} // namespace ISE
