// Hand-recovered bodies for original group o-22e518abce9579acfc03.
// f-00ff99f8e5eae4001099 — subobject-view material getter.
extern "C" void * pirates_particles_material_thunk_00ff99f8e5eae4001099(void * a0)
    __asm__("__ZThn12_N3ISE12ISEParticles11GetMaterialEv");
extern "C" void * pirates_particles_material_thunk_00ff99f8e5eae4001099(void * a0) {
    void * owner = *reinterpret_cast<void **>(static_cast<unsigned char *>(a0) + 0x38);
    return static_cast<unsigned char *>(owner) + 8;
}
