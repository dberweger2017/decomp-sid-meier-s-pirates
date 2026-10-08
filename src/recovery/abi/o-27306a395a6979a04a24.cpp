// Original compilation group o-27306a395a6979a04a24.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-b99fb99cd232ca0e9d63 — method-forwarder
// ISE::ISEAnimation::Pose(float)
// Calls: ISE::ISEAnimation::UpdateNodeList(float)
extern "C" void pirates_method_forwarder_b99fb99cd232ca0e9d63_target(void *, float)
    __asm__("__ZN3ISE12ISEAnimation14UpdateNodeListEf");
extern "C" void pirates_method_forwarder_b99fb99cd232ca0e9d63(void * a0, float a1)
    __asm__("__ZN3ISE12ISEAnimation4PoseEf");
extern "C" void pirates_method_forwarder_b99fb99cd232ca0e9d63(void * a0, float a1) {
    pirates_method_forwarder_b99fb99cd232ca0e9d63_target(a0, a1);
}
