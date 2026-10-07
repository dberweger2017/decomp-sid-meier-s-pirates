# Historical compiler feasibility

The matching compiler is **not validated**. A reproducible Linux environment and source-build experiment are present; a validated compiler/runtime image is not. `config/compiler.json` deliberately has a null container image. `doctor` reports this limitation, and candidate builds cannot receive verified-match status through a fallback compiler.

## Evidence

The archive’s Info.plist names LLVM-GCC 4.2, Xcode 4.3.2 (4E2002), and iOS SDK 5.1 (9B176). This identifies the app target’s compiler family. It does not prove the compiler patch, backend revision, per-unit flags, or compilers used for linked static libraries.

The frontend experiment starts with Apple’s [llvmgcc42-2336.9 source](https://github.com/apple-oss-distributions/llvmgcc42/tree/llvmgcc42-2336.9), commit `c92700f7f0438a4bd5084145b9f351de63256e66`. The tested backend hypothesis is the separately published [llvmCore-2326.12](https://github.com/apple-oss-distributions/llvmCore/tree/llvmCore-2326.12), commit `a250b96ad7af34aa6099535d78dc0a71d57d856e`. The frontend archive also contains a bundled LLVM core; the relationship to the shipped Xcode backend has not been established. Neither source choice proves exact compiler equivalence.

The experiment ran Linux amd64 under Docker on a macOS arm64 host. The base image is pinned by digest; the installed package inventory is locked and checked. Compiler source archives are pinned by commit and SHA-256 in `toolchain/sources.lock.json`.

Recorded stages in `toolchain/feasibility.json`:

1. CMake failed on stale source lists (`ModuleProvider.cpp`) and an absent test directory. Switched to the documented configure/make route.
2. GCC 5 exposed missing host includes for `ptrdiff_t` and `lseek64`. Explicit host include flags fixed these errors. The archived module-link rule used `-module`; a recorded host-only change uses Linux `-shared` instead.
3. The LLVM ARM backend, libraries, `llc`, `llvm-config`, and unit-test binaries built. `install-libs` avoids documentation generation requiring `groff`.
4. The next attempt ended with Docker’s `error waiting for container: unexpected EOF`, followed by disappearance of the daemon socket. No GCC frontend, ARM/Thumb Mach-O object probes, or repeatable historical candidate objects were validated.

The scripts preserve these host portability changes. They do not modify ARM code generation. Build logs are in the ignored local `build/toolchain/` cache; concise evidence is tracked separately. Do not infer feasibility of the full cross-build from the LLVM core build alone.

## Reproduce the investigation

```sh
python tools/toolchain.py fetch
python tools/toolchain.py build
python tools/toolchain.py status
```

`fetch` verifies both archived sources. `build` creates the pinned research environment and invokes `toolchain/build-legacy.sh`, retaining logs and a failure record. It does not silently install a replacement compiler or mark a toolchain validated. A manual GitHub workflow runs the same experiment on native Linux.

The build script currently tests the frontend after LLVM installation. A Darwin ARM assembler/object-emission path still needs to be supplied and pinned: the GCC driver cannot be assumed to use a compatible integrated assembler, and GNU `as` is not a Mach-O assembler. A freestanding C/C++ smoke probe can establish the basic cross-build before any SDK-dependent game work. UIKit/Foundation/C++ SDK headers must be supplied locally when candidates require them.

## Validation and eventual runtime image

After a working frontend and assembler are available, package their complete installed artifacts and runtime libraries into a Linux amd64 image. Keep the installation paths used by the compiler. The runtime must include the historical frontend/backend/assembler, not a modern Clang substitution. Retain compiler source, licenses, source/patch hashes, and binary fingerprints with the package.

Pin `config/compiler.json`’s `container.image` to an immutable image digest, supply the declared compiler path, and run:

```sh
python tools/validate_compiler.py
python configure.py
python tools/dev.py doctor
```

The validator rejects Clang and plain GCC. It requires LLVM-GCC 4.2.1 / LLVM build 2336.9 identification, actual C++ compilation, ARM and Thumb Mach-O output, an included-header depfile, and identical object hashes from two build directories. A failed validator leaves validation false or absent. Its generated evidence is tied to the compiler image/binary fingerprint and invocation; source-build feasibility still does not prove byte-for-byte equivalence with the app’s historical compiler.

The prepared container adapter mounts the project at `/work`, an optional local SDK at `/sdk`, and selects `linux/amd64`. Docker supports this on Linux and macOS; Apple Silicon requires emulation. This adapter has not been exercised with a validated historical image. Validation evidence can be supplied to the CI runner with `--validation`.

Apple’s GCC frontend is distributed under GPLv2-family terms; LLVM has its own open-source license and notices. SDK files have separate terms and are not bundled in this repository or its artifacts. No Apple account credentials are accessed by these tools.
