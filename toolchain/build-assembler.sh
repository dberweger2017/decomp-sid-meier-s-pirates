#!/bin/sh
# Build the pinned legacy ARM GAS port with GCC, without Clang code generation.
set -eu
SRC=/work/build/toolchain/cctools-port-92671d64f56fb903e000b88fb97f13fccbdf413a/cctools
PREFIX=/work/build/toolchain/legacy/install
cd "$SRC"
python - <<'PYFIX'
p='configure.ac'
original=open(p).read()
s=original.replace('CC=clang', 'CC=gcc').replace('CXX=clang++', 'CXX=g++').replace('CPP="clang -E"', 'CPP="gcc -E"')
for macro in ('AC_PROG_CC', 'AC_PROG_OBJC', 'AM_PROG_AS'):
    s=s.replace(macro+'([clang])', macro+'([gcc])')
s=s.replace('AC_PROG_CXX([clang++])', 'AC_PROG_CXX([g++])')
if s != original:
    open(p,'w').write(s)
PYFIX
autoreconf -fi
./configure --prefix="$PREFIX" --target=arm-apple-darwin11 --disable-shared
# The port supplies this visibility definition for non-Apple hosts.
make -C libstuff -j2 CFLAGS='-O2 -include ../include/foreign/extern.h'
make -C as/arm -j2 CFLAGS='-O2 -D__LITTLE_ENDIAN__=1 -include ../../include/foreign/extern.h'
make -C as/arm install
# Use relative links to the ARM assembler, avoiding the wrapper's absolute
# libexec path when packaging the installation into /opt/pirates.
mkdir -p "$PREFIX/arm-apple-darwin11/bin" "$PREFIX/bin"
ln -sf ../../libexec/as/arm/as "$PREFIX/arm-apple-darwin11/bin/as"
ln -sf ../libexec/as/arm/as "$PREFIX/bin/arm-apple-darwin11-as"
