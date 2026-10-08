// Original compilation group o-c61b01e5013c9ce55569.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-f7e7c89b258a3659e8b2 — method-forwarder
// ISE::ISELinColorKey::Equal(ISE::ISEAnimationKey const&, ISE::ISEAnimationKey const&)
// Calls: ISE::ISEColorKey::Equal(ISE::ISEAnimationKey const&, ISE::ISEAnimationKey const&)
extern "C" bool pirates_method_forwarder_f7e7c89b258a3659e8b2_target(const void *, const void *)
    __asm__("__ZN3ISE11ISEColorKey5EqualERKNS_15ISEAnimationKeyES3_");
extern "C" bool pirates_method_forwarder_f7e7c89b258a3659e8b2(const void * a0, const void * a1)
    __asm__("__ZN3ISE14ISELinColorKey5EqualERKNS_15ISEAnimationKeyES3_");
extern "C" bool pirates_method_forwarder_f7e7c89b258a3659e8b2(const void * a0, const void * a1) {
    return pirates_method_forwarder_f7e7c89b258a3659e8b2_target(a0, a1);
}
