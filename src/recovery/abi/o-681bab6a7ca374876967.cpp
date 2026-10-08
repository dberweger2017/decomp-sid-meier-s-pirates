// Original compilation group o-681bab6a7ca374876967.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-094792f3b6b10419d32c — complete-destructor
// ISE::TriStrip::~TriStrip()
// Calls: ISE::TriStrip::~TriStrip()
extern "C" void pirates_complete_destructor_094792f3b6b10419d32c_target(void *)
    __asm__("__ZN3ISE8TriStripD2Ev");
extern "C" void pirates_complete_destructor_094792f3b6b10419d32c(void * a0)
    __asm__("__ZN3ISE8TriStripD1Ev");
extern "C" void pirates_complete_destructor_094792f3b6b10419d32c(void * a0) {
    pirates_complete_destructor_094792f3b6b10419d32c_target(a0);
}

// f-30b7177e10ee3da98555 — method-forwarder
// ISE::TriStrip::Render()
// Calls: ISE::ISERenderObject::Render()
extern "C" void pirates_method_forwarder_30b7177e10ee3da98555_target(void *)
    __asm__("__ZN3ISE15ISERenderObject6RenderEv");
extern "C" void pirates_method_forwarder_30b7177e10ee3da98555(void * a0)
    __asm__("__ZN3ISE8TriStrip6RenderEv");
extern "C" void pirates_method_forwarder_30b7177e10ee3da98555(void * a0) {
    pirates_method_forwarder_30b7177e10ee3da98555_target(a0);
}
