// Original compilation group o-feb30fcaaafbd5cb88dc.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-79ade390df66961290aa — complete-destructor
// PVRShell::~PVRShell()
// Calls: PVRShell::~PVRShell()
extern "C" void pirates_complete_destructor_79ade390df66961290aa_target(void *)
    __asm__("__ZN8PVRShellD2Ev");
extern "C" void pirates_complete_destructor_79ade390df66961290aa(void * a0)
    __asm__("__ZN8PVRShellD1Ev");
extern "C" void pirates_complete_destructor_79ade390df66961290aa(void * a0) {
    pirates_complete_destructor_79ade390df66961290aa_target(a0);
}

// f-e7fdf9b3c588cf30030d — method-forwarder
// PVRShellInit::TouchPointFlushed()
// Calls: GamePad::KeyFlushed()
extern "C" void pirates_method_forwarder_e7fdf9b3c588cf30030d_target(void)
    __asm__("__ZN7GamePad10KeyFlushedEv");
extern "C" void pirates_method_forwarder_e7fdf9b3c588cf30030d(void * a0)
    __asm__("__ZN12PVRShellInit17TouchPointFlushedEv");
extern "C" void pirates_method_forwarder_e7fdf9b3c588cf30030d(void * a0) {
    pirates_method_forwarder_e7fdf9b3c588cf30030d_target();
}
