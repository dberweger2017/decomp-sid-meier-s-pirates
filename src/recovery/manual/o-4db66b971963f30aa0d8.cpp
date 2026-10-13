// Hand-recovered bodies for original group o-4db66b971963f30aa0d8.
unsigned char pirates_rev_hint[80] __asm__("_RevHint") __attribute__((aligned(16)));
unsigned char pirates_rev_content[72] __asm__("_RevContent") __attribute__((aligned(16)));

// f-b7451cb12ec14446eb95 — reads the quest hint's wanted field.
extern "C" int pirates_get_wanted(void) __asm__("__Z9getWantedv");
extern "C" int pirates_get_wanted(void) {
    return *reinterpret_cast<int *>(pirates_rev_hint + 0x0c);
}

// f-d77deea5569528f01f58 — returns the current revision's finished flag.
extern "C" unsigned char pirates_is_current_rev_finished(void)
    __asm__("__Z20isCurrentRevFinishedv");
extern "C" unsigned char pirates_is_current_rev_finished(void) {
    return pirates_rev_content[0];
}
