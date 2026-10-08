#pragma once

// Partial original FAudioSystem API. The deleting destructor proves a virtual
// destructor; remaining virtual slots, state and hierarchy are not recovered.
// Update's constant success result uses a provisional bool return type because
// the mangled symbol does not encode it. No instances or complete vtable emitted.
class FAudioSystem {
public:
    enum ESoundType { ESoundTypeUnknown = -1 };
    virtual ~FAudioSystem();
    bool Update(float elapsed);
    void SetAudioSystemType();
};
