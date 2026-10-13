// Controlled source/return-type/load-width variants. Unencoded C++ return types
// and volatile qualifiers are unknown; these are NOT source-recovery claims.
extern "C" {
unsigned alpha_unsigned(const unsigned *flags) { return (*flags >> 15) & 1; }
unsigned alpha_signed(const int *flags) { return (*flags >> 15) & 1; }
bool alpha_bool(const unsigned *flags) { return (*flags >> 15) & 1; }
bool alpha_mask(const unsigned *flags) { return (*flags & 0x8000) != 0; }
unsigned alpha_volatile_unsigned(const volatile unsigned *flags) { return (*flags >> 15) & 1; }
bool alpha_volatile_bool(const volatile unsigned *flags) { return (*flags >> 15) & 1; }
}
namespace ISE {
class ISETexture {
    unsigned char unknown[44];
    volatile unsigned flags;
public:
    bool IsAlphaEnabled();
};
bool ISETexture::IsAlphaEnabled() { return (flags >> 15) & 1; }
}
