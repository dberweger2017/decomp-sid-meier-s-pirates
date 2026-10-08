#pragma once
#include "../powervr/PVRTMathTypes.h"

namespace ISE {
// Partial target ABI from GetNodeMatrix and the sequence array destructor.
// Full channel layouts and construction behavior are being recovered separately.
class KeyframeController {
public:
    KeyframeController();
    virtual ~KeyframeController();
    void GetScaleMatrix(float time, PVRTMATRIXf &matrix);
    void GetRotationMatrix(float time, PVRTMATRIXf &matrix);
    void GetTranslationMatrix(float time, PVRTMATRIXf &matrix);
private:
    unsigned char m_unrecovered[96];
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
    char *m_name;                       // +0x00
    unsigned int m_unknown04;           // +0x04
    unsigned int m_unknown08;           // +0x08
    int m_propertyCount;                // +0x0c
    Node *m_properties;                 // +0x10, 8-byte records
    int m_nodeCount;                    // +0x14
    Node *m_nodes;                       // +0x18
    KeyframeController *m_keyframes;    // +0x1c, 100-byte objects
    unsigned char m_unrecovered20[36];  // +0x20..+0x43
    float m_time;                       // +0x44
};
} // namespace ISE
