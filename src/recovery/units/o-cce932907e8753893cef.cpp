// Original compilation group o-cce932907e8753893cef.
// f-08aa7183db1262113c80 — retain a non-null controller pointer.
extern "C" void pirates_movie_player_ctor_08aa7183db1262113c80(void *, void *)
    __asm__("__ZN14MacMoviePlayerC1EP18MyMPViewController");
extern "C" void pirates_movie_player_ctor_08aa7183db1262113c80(void *self, void *controller) {
    if (controller) *reinterpret_cast<void **>(self) = controller;
}
