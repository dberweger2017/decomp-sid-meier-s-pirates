// Original compilation group o-e3c04b8f58296776dbea.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-62aafca13788feab33d6 — method-forwarder
// ISE::ISEEditableMesh::Render()
// Calls: ISE::ISERenderObject::Render()
extern "C" void pirates_method_forwarder_62aafca13788feab33d6_target(void *)
    __asm__("__ZN3ISE15ISERenderObject6RenderEv");
extern "C" void pirates_method_forwarder_62aafca13788feab33d6(void * a0)
    __asm__("__ZN3ISE15ISEEditableMesh6RenderEv");
extern "C" void pirates_method_forwarder_62aafca13788feab33d6(void * a0) {
    pirates_method_forwarder_62aafca13788feab33d6_target(a0);
}

// f-81c5e9fc0aa556bfff02 — complete-constructor
// ISE::ISEEditableMesh::ISEEditableMesh()
// Calls: ISE::ISEEditableMesh::ISEEditableMesh()
extern "C" void pirates_complete_constructor_81c5e9fc0aa556bfff02_target(void *)
    __asm__("__ZN3ISE15ISEEditableMeshC2Ev");
extern "C" void pirates_complete_constructor_81c5e9fc0aa556bfff02(void * a0)
    __asm__("__ZN3ISE15ISEEditableMeshC1Ev");
extern "C" void pirates_complete_constructor_81c5e9fc0aa556bfff02(void * a0) {
    pirates_complete_constructor_81c5e9fc0aa556bfff02_target(a0);
}

// f-a77090de2965eec5d727 — adjusted subobject getter.
// The original thunk adds 0x7c to the incoming subobject view.
extern "C" void * pirates_adjusted_getter_a77090de2965eec5d727(void * a0)
    __asm__("__ZThn180_N3ISE15ISEEditableMesh11GetMaterialEv");
extern "C" void * pirates_adjusted_getter_a77090de2965eec5d727(void * a0) {
    return static_cast<unsigned char *>(a0) + 0x7c;
}

// f-a9e97dd7f5dfce23f8f3 — complete-destructor
// ISE::ISEEditableMesh::~ISEEditableMesh()
// Calls: ISE::ISEEditableMesh::~ISEEditableMesh()
extern "C" void pirates_complete_destructor_a9e97dd7f5dfce23f8f3_target(void *)
    __asm__("__ZN3ISE15ISEEditableMeshD2Ev");
extern "C" void pirates_complete_destructor_a9e97dd7f5dfce23f8f3(void * a0)
    __asm__("__ZN3ISE15ISEEditableMeshD1Ev");
extern "C" void pirates_complete_destructor_a9e97dd7f5dfce23f8f3(void * a0) {
    pirates_complete_destructor_a9e97dd7f5dfce23f8f3_target(a0);
}
