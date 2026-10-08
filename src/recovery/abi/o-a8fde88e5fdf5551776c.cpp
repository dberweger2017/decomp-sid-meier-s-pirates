// Original compilation group o-a8fde88e5fdf5551776c.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-2223b1acb802664aa374 — method-forwarder
// ISE::ISEPSysModifierCtlr::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISETimeController::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_2223b1acb802664aa374_target(void *, void *)
    __asm__("__ZN3ISE17ISETimeController10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_2223b1acb802664aa374(void * a0, void * a1)
    __asm__("__ZN3ISE19ISEPSysModifierCtlr10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_2223b1acb802664aa374(void * a0, void * a1) {
    pirates_method_forwarder_2223b1acb802664aa374_target(a0, a1);
}
