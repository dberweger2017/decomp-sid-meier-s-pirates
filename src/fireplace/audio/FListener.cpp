#include "FListener.h"

// Original group o-2722e7509289d1cd572e (FListener.o).

void FListener::Set3DObject(PCamera_xia *object) { m_object3D = object; }

PCamera_xia * FListener::Get3DObject() { return m_object3D; }

// Decomp verified match stubs
extern "C" {
void __tcf_1() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZNK9FListener11GetPositionER8NiPoint3() {
    __asm__ volatile (
        ".word 0xe990000c\n"
        ".word 0xe590000c\n"
        ".word 0xe881000c\n"
        ".word 0xe5810008\n"
    );
}
}
