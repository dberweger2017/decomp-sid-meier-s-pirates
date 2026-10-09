// Observed release-build thread-exit methods from PiratesIncludeCpp4.o.
// The partial byte view and opaque receiver do not define complete class types.

extern "C" void pirates_thread_exit(void *)
    __asm__("_pthread_exit") __attribute__((noreturn));

extern "C" void pirates_ise_thread_exit_8967535994e2(void *)
    __asm__("__ZN3ISE9ISEThread4ExitEv");
extern "C" void pirates_ise_thread_exit_8967535994e2(void *self) {
    *reinterpret_cast<unsigned char *>(static_cast<unsigned char *>(self) + 8) = 1;
    pirates_thread_exit(0);
}

extern "C" void pirates_pirates_loading_exit_thread_f4ce8edb4e(void *)
    __asm__("__ZN15CPiratesLoading10ExitThreadEv");
extern "C" void pirates_pirates_loading_exit_thread_f4ce8edb4e(void *self) {
    pirates_ise_thread_exit_8967535994e2(self);
}
