// Original compilation group o-cb895c66bb863be3e3db.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-1d573d92aab8c210cc4d — registration-forwarder
// _GLOBAL__I__ZN3ISE18ISEAlphaController6UpdateEf
// Calls: ISE::ISEFloatKey::RegisterLoader()
extern "C" void pirates_registration_forwarder_1d573d92aab8c210cc4d_target(void)
    __asm__("__ZN3ISE11ISEFloatKey14RegisterLoaderEv");
static void pirates_registration_forwarder_1d573d92aab8c210cc4d(void)
    __asm__("__GLOBAL__I__ZN3ISE18ISEAlphaController6UpdateEf");
static void pirates_registration_forwarder_1d573d92aab8c210cc4d(void) {
    pirates_registration_forwarder_1d573d92aab8c210cc4d_target();
}

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_registration_forwarder_1d573d92aab8c210cc4d_source_reference)(void) = pirates_registration_forwarder_1d573d92aab8c210cc4d;

// f-2343a191fd82aec74a56 — method-forwarder
// ISE::ISEAlphaController::LinkObject(ISE::ISEParticleEntity&)
// Calls: ISE::ISEFloatController::LinkObject(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_2343a191fd82aec74a56_target(void *, void *)
    __asm__("__ZN3ISE18ISEFloatController10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_2343a191fd82aec74a56(void * a0, void * a1)
    __asm__("__ZN3ISE18ISEAlphaController10LinkObjectERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_2343a191fd82aec74a56(void * a0, void * a1) {
    pirates_method_forwarder_2343a191fd82aec74a56_target(a0, a1);
}

// f-b74f42298952ebe2c71e — method-forwarder
// ISE::ISEAlphaController::LoadBinary(ISE::ISEParticleEntity&)
// Calls: ISE::ISEFloatController::LoadBinary(ISE::ISEParticleEntity&)
extern "C" void pirates_method_forwarder_b74f42298952ebe2c71e_target(void *, void *)
    __asm__("__ZN3ISE18ISEFloatController10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_b74f42298952ebe2c71e(void * a0, void * a1)
    __asm__("__ZN3ISE18ISEAlphaController10LoadBinaryERNS_17ISEParticleEntityE");
extern "C" void pirates_method_forwarder_b74f42298952ebe2c71e(void * a0, void * a1) {
    pirates_method_forwarder_b74f42298952ebe2c71e_target(a0, a1);
}
