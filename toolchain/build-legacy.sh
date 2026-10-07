#!/bin/sh
# Invoked inside the feasibility container. Never substitutes Clang.
set -eu
SRC=/work/build/toolchain/llvmgcc42-c92700f7f0438a4bd5084145b9f351de63256e66
CORE="$SRC/llvmCore"
OUT=/work/build/toolchain/legacy
mkdir -p "$OUT/core-bundled" "$OUT/gcc"
STAGE=${1:-all}
case "$STAGE" in all|frontend) ;; *) echo 'Expected all or frontend' >&2; exit 2 ;; esac
# Host-only fixes for this archived build system; no ARM backend edits.
python - "$CORE/Makefile.rules" <<'PYFIX'
import sys
p=sys.argv[1]
original=open(p).read()
s=original.replace('LD.Flags += -module', 'LD.Flags += -shared')
if s != original:
    open(p,'w').write(s)
PYFIX
if [ "$STAGE" = all ]; then
cd "$OUT/core-bundled"
"$CORE/configure" --enable-targets=arm --enable-optimized --disable-assertions --disable-docs --prefix="$OUT/install" CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
make -j2 CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
make install-libs CXXFLAGS='-std=gnu++98 -include cstddef -include unistd.h -D_LARGEFILE64_SOURCE'
printf '%s\n' "$CORE" > "$OUT/backend-source.txt"
fi
test -x "$OUT/install/bin/llvm-config"
test "$(cat "$OUT/backend-source.txt")" = "$CORE"
cd "$OUT/gcc"
CFLAGS="-O2 -fgnu89-inline" CXXFLAGS="-O2 -std=gnu++98 -include cstddef" "$SRC/configure" --build=x86_64-unknown-linux-gnu --host=x86_64-unknown-linux-gnu --target=arm-apple-darwin11 --enable-llvm="$OUT/install" --enable-languages=c,c++ --disable-bootstrap --disable-multilib --disable-shared --disable-nls --disable-werror --without-headers --prefix="$OUT/install" --program-prefix=llvm-
make -j2 all-gcc LLVM_VERSION_INFO=2336.9 CXX='g++ -std=gnu++98 -include cstddef -fpermissive' FLEX=flex BISON=bison
make install-gcc
