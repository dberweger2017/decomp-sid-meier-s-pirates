// Original compilation group o-e3c04b8f58296776dbea.
// Partial ABI leaf bodies. Full types and unencoded results remain hypotheses.
// No instruction or original-byte bodies; asm declarations name linkage only.

// f-00d1dbfa397338716018 — constant-body
// non-virtual thunk to ISE::ISEEditableMesh::UpdateUnit()
extern "C" bool pirates_leaf_00d1dbfa397338716018(void * a0)
    __asm__("__ZThn180_N3ISE15ISEEditableMesh10UpdateUnitEv");
extern "C" bool pirates_leaf_00d1dbfa397338716018(void * a0) { return 1; }

// f-9ac42c3ffcf795bb6370 — empty-body
// ISE::ISERenderUnit::AddToDrawList()
extern "C" void pirates_leaf_9ac42c3ffcf795bb6370(void * a0)
    __asm__("__ZN3ISE13ISERenderUnit13AddToDrawListEv");
extern "C" void pirates_leaf_9ac42c3ffcf795bb6370(void * a0) {  }

// This thunk sees the +180 render-unit subobject: +252 - 180 = +72.
struct pirates_leaf_9e2970e036f64c211a3a_subobject { unsigned char unknown[72]; unsigned int vertex_count; };
// f-9e2970e036f64c211a3a — adjusted-field-getter
// non-virtual thunk to ISE::ISEEditableMesh::GetVertexNum()
extern "C" unsigned int pirates_leaf_9e2970e036f64c211a3a(void * a0)
    __asm__("__ZThn180_N3ISE15ISEEditableMesh12GetVertexNumEv");
extern "C" unsigned int pirates_leaf_9e2970e036f64c211a3a(void * a0) { return static_cast<pirates_leaf_9e2970e036f64c211a3a_subobject *>(a0)->vertex_count; }
