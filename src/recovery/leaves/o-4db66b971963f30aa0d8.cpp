// Original compilation group o-4db66b971963f30aa0d8.
// Partial ABI leaf bodies. Full types and unencoded results remain hypotheses.
// No instruction or original-byte bodies; asm declarations name linkage only.

// f-94a5e0dbbe909641398d — destruction-callback
// __tcf_4
static void pirates_leaf_94a5e0dbbe909641398d(void * a0)
    __asm__("___tcf_4");
static void pirates_leaf_94a5e0dbbe909641398d(void * a0) {  }

// Source-emission reference only; no original data or lifetime-registration credit.
extern "C" void (* const pirates_leaf_94a5e0dbbe909641398d_source_reference)(void *) = pirates_leaf_94a5e0dbbe909641398d;

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
