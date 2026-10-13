// Hand-recovered bodies for original group o-a2a2a5724cebc5bf6fbf.
// f-7f12240a376ce2437a4d — clear the first three CFIleIO state words.
extern "C" void pirates_cfileio_ctor_7f12240a376ce2437a4d(void *)
    __asm__("__ZN7CFIleIOC1Ev");
extern "C" void pirates_cfileio_ctor_7f12240a376ce2437a4d(void *self) {
    unsigned int *fields = static_cast<unsigned int *>(self);
    fields[0] = 0;
    fields[1] = 0;
    fields[2] = 0;
}
