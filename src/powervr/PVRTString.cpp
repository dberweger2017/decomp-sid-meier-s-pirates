#include "PVRTString.h"

// Original group: o-1f9e1182f8fd5b86d0bb (libOGLES2Tools.a(PVRTString.o)).
// Add only individually verified definitions. This scaffold earns no progress.

const char *CPVRTString::c_str() const { return m_buffer; }

unsigned long CPVRTString::length() const { return m_length; }

unsigned long CPVRTString::size() const { return m_length; }

const char &CPVRTString::operator[](unsigned long index) const { return m_buffer[index]; }

char &CPVRTString::operator[](unsigned long index) { return m_buffer[index]; }

bool CPVRTString::empty() const { return m_length == 0; }

CPVRTString &CPVRTString::operator=(const CPVRTString &other) {
    return assign(other.m_buffer, other.m_length);
}

// npos is the all-ones unsigned length sentinel; constructors test -1.
// Section/alignment reproduce the observed original allocation; original source
// attributes and compilation flags remain unknown. Default emission is literal4.
const unsigned long CPVRTString::npos
    __attribute__((section("__TEXT,__const"), aligned(16))) =
    static_cast<unsigned long>(-1);
