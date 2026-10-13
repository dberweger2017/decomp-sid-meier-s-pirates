// Diagnostic high-level source, never registered as recovered game source.
extern "C" {
extern unsigned global_value;
extern unsigned hidden_value __attribute__((visibility("hidden")));
unsigned defined_value;
extern int external_call(int);
extern void external_void(const float *, const float *, float *);
int tail_int(int value) { return external_call(value); }
void tail_void(float *out, const float *a, const float *b) {
    external_void(a, b, out);
}
unsigned *global_address() { return &global_value; }
unsigned *hidden_address() { return &hidden_value; }
unsigned *defined_address() { return &defined_value; }
unsigned extract_bit(unsigned value) { return (value >> 15) & 1; }
unsigned extract_field(unsigned value) { return (value >> 5) & 127; }
unsigned extract_shift_pair(unsigned value) { return (value << 16) >> 31; }
struct Bits { unsigned low : 15; unsigned alpha : 1; unsigned high : 16; };
unsigned extract_bitfield(const Bits *value) { return value->alpha; }
unsigned constant_value() { return 0x12345678U; }
unsigned add_one(unsigned value) { return value + 1; }
}

// Minimal ABI declarations for the three existing discrepancy examples.
struct PVRTMATRIXf { float f[16]; };
extern void NEON_Matrix4Mul(const float *, const float *, float *);
void PVRTMatrixMultiplyF(PVRTMATRIXf &out, const PVRTMATRIXf &a,
                         const PVRTMATRIXf &b) {
    NEON_Matrix4Mul(a.f, b.f, out.f);
}
struct NiRTTI;
class NiObject {
public:
    static const NiRTTI m_RTTI;
    const NiRTTI *GetRTTI() const;
};
const NiRTTI *NiObject::GetRTTI() const { return &m_RTTI; }
namespace ISE {
class ISETexture {
    unsigned char unknown[44];
    unsigned flags;
public:
    bool IsAlphaEnabled();
};
bool ISETexture::IsAlphaEnabled() { return (flags >> 15) & 1; }
}
