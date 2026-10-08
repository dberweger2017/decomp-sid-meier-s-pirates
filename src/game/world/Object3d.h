#pragma once

class NiNode;
// Original E3DAS_Type enumerators remain unknown.
enum E3DAS_Type { E3DAS_Unknown = -1 };

// Partial declarations for direct function comparison only. Do not instantiate.
// Complete inheritance, virtual slots, constructors and object size are unknown.
// Opaque ranges include any unrecovered base state. Unencoded return types,
// pointees and signedness remain hypotheses; matching bytes do not prove them.
class Object3d {
public:
    void SetCycleTime(float time);
    void SetDetailLevel(int level);
    void SetCopperPlating(bool enabled);
    void SetCottonSails(bool enabled);
    void SetHullShaderEnable(bool enabled);
    void MakeLightsAffectNode(NiNode *node);
    void AddZBufferProperty(bool first, bool second);
    void AddStaticEffects(Object3d *object, char *name, int index, E3DAS_Type type);
    void RemoveStaticEffects(Object3d *object, char *name);

private:
    unsigned char m_unknown_00[436];
    int m_detailLevel; // +0x1b4
    unsigned char m_unknown_1b8[148];
    float m_cycleTime; // +0x24c
};
