// Original compilation group o-97ff96143a03b7b7e331.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-efa82815d570f6bc2975 — complete-constructor
// ISE::ISEMesh::ISEMesh(int, ISE::ISEEntity*)
// Calls: ISE::ISEMesh::ISEMesh(int, ISE::ISEEntity*)
extern "C" void pirates_complete_constructor_efa82815d570f6bc2975_target(void *, int, void *)
    __asm__("__ZN3ISE7ISEMeshC2EiPNS_9ISEEntityE");
extern "C" void pirates_complete_constructor_efa82815d570f6bc2975(void * a0, int a1, void * a2)
    __asm__("__ZN3ISE7ISEMeshC1EiPNS_9ISEEntityE");
extern "C" void pirates_complete_constructor_efa82815d570f6bc2975(void * a0, int a1, void * a2) {
    pirates_complete_constructor_efa82815d570f6bc2975_target(a0, a1, a2);
}
