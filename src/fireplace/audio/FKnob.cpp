#include "FKnob.h"

// Original group: o-67cbdb66049bec401875 (FKnob.o).
// Only explicitly recovered functions are implemented; static pool remains missing.

FKnob::FKnob() : m_volume(1.0f), m_next(0) {}
FKnob::~FKnob() {}
void FKnob::Clear() { m_volume=1.0f; m_next=0; }
void FKnob::SetVolume(float volume) { m_volume=volume; }
float FKnob::GetVolume() { return m_volume; }
void FKnob::AddKnob(FKnob *knob) {
    if (!knob) return;
    FKnob *current=this;
    FKnob *last;
    do {
        last=current;
        current=current->m_next;
    } while (current);
    last->m_next=knob;
}

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}
