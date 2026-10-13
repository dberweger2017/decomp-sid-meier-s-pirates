// Return types are absent from the C++ symbol name and retained STABS map.
// Test an unsigned 0/1 return hypothesis WITHOUT volatile memory semantics.
namespace ISE {
class ISETexture {
    unsigned char unknown[44];
    unsigned flags;
public:
    unsigned IsAlphaEnabled();
};
unsigned ISETexture::IsAlphaEnabled() { return (flags >> 15) & 1; }
}
