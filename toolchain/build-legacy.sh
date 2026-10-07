#!/bin/sh
# Invoked inside the feasibility container. Never substitutes Clang.
set -eu
SRC=/work/build/toolchain/llvmgcc42-c92700f7f0438a4bd5084145b9f351de63256e66
CORE=/work/build/toolchain/llvmCore-a250b96ad7af34aa6099535d78dc0a71d57d856e
OUT=/work/build/toolchain/legacy
mkdir -p "$OUT/core" "$OUT/gcc"
# Host-only fixes for this archived build system; no ARM backend edits.
python - "$CORE/Makefile.rules" <<'PYFIX'
import sys
p=sys.argv[1]
s=open(p).read().replace('LD.Flags += -module', 'LD.Flags += -shared')
open(p,'w').write(s)
PYFIX
cd "$OUT/core"
"$CORE/configure" --enable-targets=arm --enable-optimized --disable-assertions --disable-docs --prefix="$OUT/install" CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
make -j2 CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
make install-libs CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
cd "$OUT/gcc"
CFLAGS="-O2 -fgnu89-inline" CXXFLAGS="-O2 -std=gnu++98 -include cstddef" "$SRC/configure" --build=x86_64-unknown-linux-gnu --host=x86_64-unknown-linux-gnu --target=arm-apple-darwin11 --enable-llvm="$OUT/install" --enable-languages=c,c++ --disable-bootstrap --disable-multilib --disable-shared --disable-nls --disable-werror --without-headers --prefix="$OUT/install" --program-prefix=llvm-
make -j2 all-gcc LLVM_VERSION_INFO=2336.9
make install-gcc
