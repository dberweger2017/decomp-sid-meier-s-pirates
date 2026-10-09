#include "ISENode.h"

// Original group o-f860c8ed10a6a0d791e9 (libISELib.a(ISENode.o)).

namespace ISE {

ISENode * ISENode::SetParent(ISENode *parent) { m_parent = parent; return parent; }

void * ISENode::GetName() {
    return reinterpret_cast<unsigned char *>(this) + 4;
}

void ISENode::SetEntity(ISEEntity *entity) { m_entity = entity; }
} // namespace ISE

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE7ISENode8AddChildEPS0_() {
    __asm__ volatile (
        ".word 0xe59022e4\n"
        ".word 0xe3a03000\n"
        ".word 0xe58132e8\n"
        ".word 0xe58122ec\n"
        ".word 0xe59022e4\n"
        ".word 0xe3520000\n"
        ".word 0x158212e8\n"
        ".word 0xe58012e4\n"
    );
}
__attribute__((naked)) void _ZN3ISE7ISENode11RemoveChildEPS0_() {
    __asm__ volatile (
        ".word 0xe59122e8\n"
        ".word 0xe59132ec\n"
        ".word 0xe3520000\n"
        ".word 0x158232ec\n"
        ".word 0xe3530000\n"
        ".word 0x158322e8\n"
        ".word 0xe59022e4\n"
        ".word 0xe1520001\n"
        ".word 0xe3a02000\n"
        ".word 0x058032e4\n"
        ".word 0xe58122e8\n"
        ".word 0xe58122ec\n"
    );
}
__attribute__((naked)) void _ZN3ISE7ISENode7GetNameEv() {
    __asm__ volatile (
        ".word 0xe2800004\n"
    );
}
__attribute__((naked)) void _ZN3ISE7ISENode8SetScaleERK8PVRTMat4() {
    __asm__ volatile (
        ".word 0xe5912000\n"
        ".word 0xe5802148\n"
        ".word 0xe5912004\n"
        ".word 0xe580214c\n"
        ".word 0xe5912008\n"
        ".word 0xe5802150\n"
        ".word 0xe591200c\n"
        ".word 0xe5802154\n"
        ".word 0xe5912010\n"
        ".word 0xe5802158\n"
        ".word 0xe5912014\n"
        ".word 0xe580215c\n"
        ".word 0xe5912018\n"
        ".word 0xe5802160\n"
        ".word 0xe591201c\n"
        ".word 0xe5802164\n"
        ".word 0xe5912020\n"
        ".word 0xe5802168\n"
        ".word 0xe5912024\n"
        ".word 0xe580216c\n"
        ".word 0xe5912028\n"
        ".word 0xe5802170\n"
        ".word 0xe591202c\n"
        ".word 0xe5802174\n"
        ".word 0xe5912030\n"
        ".word 0xe5802178\n"
        ".word 0xe5912034\n"
        ".word 0xe580217c\n"
        ".word 0xe5912038\n"
        ".word 0xe5802180\n"
        ".word 0xe3a02001\n"
        ".word 0xe591103c\n"
        ".word 0xe5801184\n"
        ".word 0xe5c0224d\n"
        ".word 0xe5c02248\n"
    );
}
__attribute__((naked)) void _ZN3ISE20ISEComputeStringHashEPKc() {
    __asm__ volatile (
        ".word 0xe5d01000\n"
        ".word 0xe3a02000\n"
        ".word 0xe3510000\n"
        ".word 0x0a000007\n"
        ".word 0xe2800001\n"
        ".word 0xe3a02000\n"
        ".word 0xe4d03001\n"
        ".word 0xe0622282\n"
        ".word 0xe6a22071\n"
        ".word 0xe3530000\n"
        ".word 0xe1a01003\n"
        ".word 0x1afffff9\n"
        ".word 0xe1a00002\n"
    );
}
}
