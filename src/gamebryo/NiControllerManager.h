#pragma once
#include "../recovery/ReleaseHookTypes.h"

// Partial interface for observed shipped release hooks. Do not instantiate.
// Hierarchy, virtual slots, full layout and unencoded return types are unknown.
// Empty definitions reproduce observed no-op bodies; they are not placeholders
// for unrecovered behavior and do not imply other platforms used these bodies.
class NiControllerManager {
public:
    void Update(float);
    void RemoveAllSequences();
    void DeactivateAll();
    void SetGlobalTransform(NiMatrix3 const&, NiPoint3 const&);
    void ProcessClone(NiCloningProcess&);
    void SaveBinary(NiStream&);
    void LoadBinary(NiStream&);
    void LinkObject(NiStream&);
    void PostLinkObject(NiStream&);
    void Start(float);
    void Start();
    void Stop();
    void SetTarget(NiObjectNET*);
    void Blend(float, float, bool);
    bool CanBeExtracted() const;
};
