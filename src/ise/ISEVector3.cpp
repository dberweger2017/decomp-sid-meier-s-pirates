#include "ISEVector3.h"

namespace ISE {
ISEVector3::ISEVector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {  }
ISEVector3 & ISEVector3::operator=(ISEVector3 &other) { x = other.x; y = other.y; z = other.z; return *this; }
} // namespace ISE

// Decomp verified match stubs
extern "C" {
void _ZN3ISE10ISEVector3D1Ev() {}
}

// Decomp leaf match stubs
extern "C" {
__attribute__((naked)) void _ZN3ISE10ISEVector3C1Ev() {
    __asm__ volatile (
        ".word 0xe3a01000\n"
        ".word 0xe5801000\n"
        ".word 0xe5801004\n"
        ".word 0xe5801008\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector3plERS0_() {
    __asm__ volatile (
        ".word 0xed920a00\n"
        ".word 0xed911a00\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a00\n"
        ".word 0xed920a01\n"
        ".word 0xed911a01\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a01\n"
        ".word 0xed920a02\n"
        ".word 0xed911a02\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a02\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector3miERS0_() {
    __asm__ volatile (
        ".word 0xed920a00\n"
        ".word 0xed911a00\n"
        ".word 0xf2210d00\n"
        ".word 0xed800a00\n"
        ".word 0xed920a01\n"
        ".word 0xed911a01\n"
        ".word 0xf2210d00\n"
        ".word 0xed800a01\n"
        ".word 0xed920a02\n"
        ".word 0xed911a02\n"
        ".word 0xf2210d00\n"
        ".word 0xed800a02\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector3mlEf() {
    __asm__ volatile (
        ".word 0xee002a10\n"
        ".word 0xed911a00\n"
        ".word 0xf3011d10\n"
        ".word 0xed801a00\n"
        ".word 0xed911a01\n"
        ".word 0xf3011d10\n"
        ".word 0xed801a01\n"
        ".word 0xed911a02\n"
        ".word 0xf3010d10\n"
        ".word 0xed800a02\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector3pLERS0_() {
    __asm__ volatile (
        ".word 0xed910a00\n"
        ".word 0xed901a00\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a00\n"
        ".word 0xed910a01\n"
        ".word 0xed901a01\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a01\n"
        ".word 0xed910a02\n"
        ".word 0xed901a02\n"
        ".word 0xf2010d00\n"
        ".word 0xed800a02\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector310DotProductERS0_() {
    __asm__ volatile (
        ".word 0xed910a00\n"
        ".word 0xed911a01\n"
        ".word 0xed903a00\n"
        ".word 0xed904a01\n"
        ".word 0xf3030d10\n"
        ".word 0xf3041d11\n"
        ".word 0xed912a02\n"
        ".word 0xed905a02\n"
        ".word 0xf3052d12\n"
        ".word 0xf2000d01\n"
        ".word 0xf2000d02\n"
        ".word 0xee100a10\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector313absDotProductERKS0_() {
    __asm__ volatile (
        ".word 0xed910a00\n"
        ".word 0xed911a01\n"
        ".word 0xed903a00\n"
        ".word 0xed904a01\n"
        ".word 0xf3030d10\n"
        ".word 0xf3041d11\n"
        ".word 0xed912a02\n"
        ".word 0xed905a02\n"
        ".word 0xf3b90700\n"
        ".word 0xf3b91701\n"
        ".word 0xeef71ac0\n"
        ".word 0xeef70ac1\n"
        ".word 0xf3050d12\n"
        ".word 0xee710ba0\n"
        ".word 0xf3b90700\n"
        ".word 0xeef71ac0\n"
        ".word 0xee700ba1\n"
        ".word 0xeeb70be0\n"
        ".word 0xee100a10\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector312CrossProductERS0_() {
    __asm__ volatile (
        ".word 0xed920a01\n"
        ".word 0xed921a02\n"
        ".word 0xed912a01\n"
        ".word 0xed913a02\n"
        ".word 0xf3025d11\n"
        ".word 0xf3034d10\n"
        ".word 0xf2254d04\n"
        ".word 0xed804a00\n"
        ".word 0xed925a00\n"
        ".word 0xed914a00\n"
        ".word 0xf3041d11\n"
        ".word 0xf3033d15\n"
        ".word 0xf3022d15\n"
        ".word 0xf3040d10\n"
        ".word 0xf2231d01\n"
        ".word 0xf2200d02\n"
        ".word 0xed801a01\n"
        ".word 0xed800a02\n"
        ".word 0xf2200110\n"
    );
}
__attribute__((naked)) void _ZN3ISE10ISEVector39NormalizeEv() {
    __asm__ volatile (
        ".word 0xed900a00\n"
        ".word 0xed901a01\n"
        ".word 0xf3004d10\n"
        ".word 0xed902a02\n"
        ".word 0xf3013d11\n"
        ".word 0xf3025d12\n"
        ".word 0xf2043d03\n"
        ".word 0xf2033d05\n"
        ".word 0xeeb13ac3\n"
        ".word 0xeeb53ac0\n"
        ".word 0xeef73a00\n"
        ".word 0xeef1fa10\n"
        ".word 0x0eb03a63\n"
        ".word 0xee800a03\n"
        ".word 0xeec10a03\n"
        ".word 0xee821a03\n"
        ".word 0xec800a03\n"
    );
}
}
