// Hand-recovered bodies for original group o-681bab6a7ca374876967.
// f-e2dc408ce166ce31c051 — adjusted subobject getter.
// The original thunk adds 0x20 to the incoming subobject view.
extern "C" void * pirates_adjusted_getter_e2dc408ce166ce31c051(void * a0)
    __asm__("__ZThn180_N3ISE8TriStrip11GetMaterialEv");
extern "C" void * pirates_adjusted_getter_e2dc408ce166ce31c051(void * a0) {
    return static_cast<unsigned char *>(a0) + 0x20;
}

// f-2e94ff6213c641dcde2a — vertex-count getter from the render-unit view.
extern "C" unsigned int pirates_tristrip_vertex_count_thunk_2e94ff6213c641dcde2a(void * a0)
    __asm__("__ZThn180_N3ISE8TriStrip12GetVertexNumEv");
extern "C" unsigned int pirates_tristrip_vertex_count_thunk_2e94ff6213c641dcde2a(void * a0) {
    void * vertexBuffer = *reinterpret_cast<void **>(static_cast<unsigned char *>(a0) + 0x14);
    return *reinterpret_cast<unsigned int *>(static_cast<unsigned char *>(vertexBuffer) + 0x3c);
}
