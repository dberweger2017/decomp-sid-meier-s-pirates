#!/bin/sh
# Host Clang builds the Linux linker; matching objects still use LLVM-GCC.
set -eu
SRC=/work/build/toolchain/cctools-port-92671d64f56fb903e000b88fb97f13fccbdf413a/cctools
cd "$SRC"
python - <<'PYFIX'
for p in ('ld64/src/ld/Options.cpp', 'ld64/src/ld/InputFiles.cpp'):
    s=open(p).read()
    # Linux also has sys/sysctl.h, but not Darwin's kernel version API.
    # CPU-count detection retains the port's default eight workers. Neither
    # patch changes ARM code generation, layout, bindings or iOS options.
    s=s.replace('#if __has_include(<sys/sysctl.h>)', '#if defined(__APPLE__) && __has_include(<sys/sysctl.h>)')
    open(p,'w').write(s)
PYFIX
make -C libstuff -j2 CFLAGS='-O2 -include ../include/foreign/extern.h'
make -C ld64 -j2 CC=clang-3.8 CXX=clang++-3.8
mkdir -p /work/build/linker/install/bin
cp ld64/src/ld/ld /work/build/linker/install/bin/ld
dpkg-query -W -f='${Package}=${Version}\n' > /work/build/linker/packages.lock
