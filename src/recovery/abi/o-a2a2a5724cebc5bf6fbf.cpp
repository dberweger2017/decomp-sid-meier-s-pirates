// Original compilation group o-a2a2a5724cebc5bf6fbf.
// Recovered ARM ABI entry points and one direct field initializer.
// Opaque pointer parameters describe register passing, not complete types.
// Callee implementations, full layouts and unencoded results remain separate.

// f-ef41c5da5b413a194a0b — complete-destructor
// CFIleIO::~CFIleIO()
// Calls: CFIleIO::~CFIleIO()
extern "C" void pirates_complete_destructor_ef41c5da5b413a194a0b_target(void *)
    __asm__("__ZN7CFIleIOD2Ev");
extern "C" void pirates_complete_destructor_ef41c5da5b413a194a0b(void * a0)
    __asm__("__ZN7CFIleIOD1Ev");
extern "C" void pirates_complete_destructor_ef41c5da5b413a194a0b(void * a0) {
    pirates_complete_destructor_ef41c5da5b413a194a0b_target(a0);
}

// f-7f12240a376ce2437a4d — clear the first three CFIleIO state words.
extern "C" void pirates_cfileio_ctor_7f12240a376ce2437a4d(void *)
    __asm__("__ZN7CFIleIOC1Ev");
extern "C" void pirates_cfileio_ctor_7f12240a376ce2437a4d(void *self) {
    unsigned int *fields = static_cast<unsigned int *>(self);
    fields[0] = 0;
    fields[1] = 0;
    fields[2] = 0;
}
