// Original compilation group o-64dc62a7ebf337db9675.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-00d201979f0edfadd7ce — method-forwarder
// ISE::ISEPSysSphericalCollider::Update(float, ISE::ISEPSysData*, unsigned short)
// Calls: ISE::ISEPSysCollider::Update(float, ISE::ISEPSysData*, unsigned short)
extern "C" void pirates_method_forwarder_00d201979f0edfadd7ce_target(void *, float, void *, unsigned short)
    __asm__("__ZN3ISE15ISEPSysCollider6UpdateEfPNS_11ISEPSysDataEt");
extern "C" void pirates_method_forwarder_00d201979f0edfadd7ce(void * a0, float a1, void * a2, unsigned short a3)
    __asm__("__ZN3ISE24ISEPSysSphericalCollider6UpdateEfPNS_11ISEPSysDataEt");
extern "C" void pirates_method_forwarder_00d201979f0edfadd7ce(void * a0, float a1, void * a2, unsigned short a3) {
    pirates_method_forwarder_00d201979f0edfadd7ce_target(a0, a1, a2, a3);
}
