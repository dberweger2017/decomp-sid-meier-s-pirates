// Original compilation group o-869c1fa37c759d3c89c8.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-830e216519aef8295d2f — method-forwarder
// ISE::ISEParticleGeometryData::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEParticleObject::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_830e216519aef8295d2f_target(void *, void *)
    __asm__("__ZN3ISE17ISEParticleObject10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_830e216519aef8295d2f(void * a0, void * a1)
    __asm__("__ZN3ISE23ISEParticleGeometryData10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_830e216519aef8295d2f(void * a0, void * a1) {
    pirates_method_forwarder_830e216519aef8295d2f_target(a0, a1);
}

// f-9d7d2808d381234d1c38 — complete-destructor
// ISE::ISEParticleGeometryData::~ISEParticleGeometryData()
// Calls: ISE::ISEParticleGeometryData::~ISEParticleGeometryData()
extern "C" void pirates_complete_destructor_9d7d2808d381234d1c38_target(void *)
    __asm__("__ZN3ISE23ISEParticleGeometryDataD2Ev");
extern "C" void pirates_complete_destructor_9d7d2808d381234d1c38(void * a0)
    __asm__("__ZN3ISE23ISEParticleGeometryDataD1Ev");
extern "C" void pirates_complete_destructor_9d7d2808d381234d1c38(void * a0) {
    pirates_complete_destructor_9d7d2808d381234d1c38_target(a0);
}
