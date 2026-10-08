#pragma once
#include "ISESequenceMemory.h"
#include "../powervr/PVRTMathTypes.h"
#include <string.h>

namespace ISE { namespace sequence_reader {
// The original readers assemble little-endian words from bytes: variable-length
// names leave later fields unaligned. Size validation is not present in the
// observed functions; these candidates require a valid serialized sequence.
inline __attribute__((always_inline)) unsigned int Word(const char *&cursor) {
    const unsigned char *bytes = reinterpret_cast<const unsigned char *>(cursor);
    unsigned int value = bytes[0] | (static_cast<unsigned int>(bytes[1]) << 8) |
                         (static_cast<unsigned int>(bytes[2]) << 16) | (static_cast<unsigned int>(bytes[3]) << 24);
    cursor += 4;
    return value;
}
inline __attribute__((always_inline)) float Scalar(const char *&cursor) {
    union { unsigned int bits; float scalar; } value;
    value.bits = Word(cursor);
    return value.scalar;
}
inline __attribute__((always_inline)) void Value(const char *&cursor, PVRTVECTOR3f &value) {
    value.x = Scalar(cursor); value.y = Scalar(cursor); value.z = Scalar(cursor);
}
inline __attribute__((always_inline)) void Value(const char *&cursor, PVRTQUATERNIONf &value) {
    value.x = Scalar(cursor); value.y = Scalar(cursor); value.z = Scalar(cursor); value.w = Scalar(cursor);
}
inline __attribute__((always_inline)) void Value(const char *&cursor, float &value) { value = Scalar(cursor); }
inline __attribute__((always_inline)) char *Name(const char *&cursor) {
    const int length = Word(cursor);
    if (length > 0) {
        char *result = new char[length + 1]();
        strncpy(result, cursor, length);
        cursor += length;
        return result;
    }
    return new char[1]();
}
template<class T>
inline __attribute__((always_inline)) void Channel(const char *&cursor, int &count, float *&times, T *&values, bool zeroTimes) {
    const int entries = Word(cursor);
    if (entries < 1) return;
    count = entries;
    times = zeroTimes ? new float[entries]() : new float[entries];
    values = new T[entries];
    for (int i = 0; i < entries; ++i) times[i] = Scalar(cursor);
    for (int i = 0; i < entries; ++i) Value(cursor, values[i]);
}
} } // namespace ISE::sequence_reader
