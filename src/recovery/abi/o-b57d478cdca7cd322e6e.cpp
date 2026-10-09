// Original compilation group o-b57d478cdca7cd322e6e.
// Recovered ARM ABI entry points, expressed as ordinary C++ calls.
// Opaque pointer parameters describe register passing, not complete types.
// GNU asm labels are symbol linkage only; no instruction/byte bodies.
// Callee implementations, full layouts and unencoded results remain separate.

// f-2a442440443a41ab5770 — complete-constructor
// Phono2::PAudioSystem::PAudioSystem(int, int)
// Calls: Phono2::PAudioSystem::PAudioSystem(int, int)
extern "C" void pirates_complete_constructor_2a442440443a41ab5770_target(void *, int, int)
    __asm__("__ZN6Phono212PAudioSystemC2Eii");
extern "C" void pirates_complete_constructor_2a442440443a41ab5770(void * a0, int a1, int a2)
    __asm__("__ZN6Phono212PAudioSystemC1Eii");
extern "C" void pirates_complete_constructor_2a442440443a41ab5770(void * a0, int a1, int a2) {
    pirates_complete_constructor_2a442440443a41ab5770_target(a0, a1, a2);
}

// f-b1a597b7a5ab5dc0df5a — complete-destructor
// Phono2::PAudioSystem::~PAudioSystem()
// Calls: Phono2::PAudioSystem::~PAudioSystem()
extern "C" void pirates_complete_destructor_b1a597b7a5ab5dc0df5a_target(void *)
    __asm__("__ZN6Phono212PAudioSystemD2Ev");
extern "C" void pirates_complete_destructor_b1a597b7a5ab5dc0df5a(void * a0)
    __asm__("__ZN6Phono212PAudioSystemD1Ev");
extern "C" void pirates_complete_destructor_b1a597b7a5ab5dc0df5a(void * a0) {
    pirates_complete_destructor_b1a597b7a5ab5dc0df5a_target(a0);
}
