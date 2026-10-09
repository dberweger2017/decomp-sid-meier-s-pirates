// Original compilation group o-bd0fcb65565d801a101b.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-c98272e3fef8e65bb312 — method-forwarder
// ISE::ISELinFloatKey::Equal(ISE::ISEAnimationKey const&, ISE::ISEAnimationKey const&)
// Calls: ISE::ISEFloatKey::Equal(ISE::ISEAnimationKey const&, ISE::ISEAnimationKey const&)
extern "C" bool pirates_method_forwarder_c98272e3fef8e65bb312_target(const void *, const void *)
    __asm__("__ZN3ISE11ISEFloatKey5EqualERKNS_15ISEAnimationKeyES3_");
extern "C" bool pirates_method_forwarder_c98272e3fef8e65bb312(const void * a0, const void * a1)
    __asm__("__ZN3ISE14ISELinFloatKey5EqualERKNS_15ISEAnimationKeyES3_");
extern "C" bool pirates_method_forwarder_c98272e3fef8e65bb312(const void * a0, const void * a1) {
    return pirates_method_forwarder_c98272e3fef8e65bb312_target(a0, a1);
}
