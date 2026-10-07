# Historical compiler feasibility

The LLVM-GCC hypothesis now has a **validated C/C++ ARMv7 cross-build**, tested in a pinned Linux amd64 container under Apple Silicon emulation. ARM and Thumb Mach-O probes, included-header dependencies, repeated object hashes, and the historical Ninja match/header-regression loop pass. Exact equivalence to the app's shipped compiler remains unproven. `config/compiler.json` is a template with a null image; `tools/toolchain.py build` generates a locally pinned, validated profile in `build/toolchain/compiler.json`. No modern compiler substitutes for matching compilation.

## Evidence

The archive’s Info.plist names LLVM-GCC 4.2, Xcode 4.3.2 (4E2002), and iOS SDK 5.1 (9B176). This identifies the app target’s compiler family. It does not prove the compiler patch, backend revision, per-unit flags, or compilers used for linked static libraries.

The frontend experiment starts with Apple’s [llvmgcc42-2336.9 source](https://github.com/apple-oss-distributions/llvmgcc42/tree/llvmgcc42-2336.9), commit `c92700f7f0438a4bd5084145b9f351de63256e66`, and now uses the LLVM core bundled in that archive. The separately published [llvmCore-2326.12](https://github.com/apple-oss-distributions/llvmCore/tree/llvmCore-2326.12), commit `a250b96ad7af34aa6099535d78dc0a71d57d856e`, built but failed frontend integration: it lacks `GlobalValue::LinkerPrivateWeakLinkage` and `Type::getIntNTy`. The bundled core contains both required APIs. Its relationship to the shipped Xcode backend has not been established; source-build feasibility does not prove exact compiler equivalence.

The experiment ran Linux amd64 under Docker on a macOS arm64 host. The base image is pinned by digest; the installed package inventory is locked and checked. Compiler source archives are pinned by commit and SHA-256 in `toolchain/sources.lock.json`.

Recorded stages in `toolchain/feasibility.json`:

1. CMake failed on stale source lists (`ModuleProvider.cpp`) and an absent test directory. Switched to the documented configure/make route.
2. GCC 5 exposed missing host includes for `ptrdiff_t` and `lseek64`. Explicit host include flags fixed these errors. The archived module-link rule used `-module`; a recorded host-only change uses Linux `-shared` instead.
3. The LLVM ARM backend, libraries, `llc`, `llvm-config`, and unit-test binaries built. `install-libs` avoids documentation generation requiring `groff`.
4. One attempt ended with Docker’s `error waiting for container: unexpected EOF`, followed by disappearance of the daemon socket. The user reported closing Docker; after restarting it, the cached build resumed.
5. The GCC frontend's bridge ignores configured CXXFLAGS. Passing host C++ includes through make's CXX command repairs missing `ptrdiff_t`; `-fpermissive` permits old constructor expressions in the never-called library link helper. Missing flex/bison inputs require pinned parser generators. The frontend retry remains unvalidated.
6. Source inspection confirms assembly-file emission. The pinned Linux port of cctools 845's ARM GAS built with GCC host compilation, explicit port visibility definitions and little-endian host flags. An independent Thumb-2 probe produced a valid ARMv7 Mach-O object with the expected Thumb symbol. Its revision is an assembler hypothesis, not evidence of the assembler shipped with Xcode 4.3.2.
7. The frontend reached compilation of its LLVM bridge and rejected the separate backend's missing APIs. The build now selects the frontend archive's bundled core, without target-code patches.
8. Native Linux built the bundled core but exposed omitted tool installation; `llc` and `llvm-config` are now installed explicitly. The resumed frontend on Linux exposed an old `mempcpy` declaration conflicting with glibc. A host-only `__GLIBC__` guard uses the system declaration without changing helper logic or target generation.
9. C/C++ frontends and drivers built and installed. Default `all-gcc` also attempts target libgcc and Darwin crt3, which require SDK headers. Compiler-only make flags omit those runtime/linking artifacts. This milestone validates `-c` object compilation; it provides no target runtime, SDK, linker, or full-game link. The current recipe enables C and C++; Objective-C frontends are not validated.
10. The container passed C/C++ ARM and Thumb Mach-O, header-dependency, and repeated-object probes. Its compiler source's plain `armv7` architecture omits ARM mode capability (`FL_NOTM`); explicit `armv7-a` works. This is an investigation flag, not a recovered original flag. The historical Ninja loop verified a synthetic four-byte function, rebuilt on a header edit, and detected its lost match.

The scripts preserve these host portability changes. They do not modify ARM code generation. Build logs are in the ignored local `build/toolchain/` cache; concise evidence is tracked separately. Do not infer feasibility of the full cross-build from the LLVM core build alone.

## Reproduce the investigation

```sh
python tools/toolchain.py fetch
python tools/toolchain.py build
python tools/toolchain.py status
```

`fetch` verifies the frontend, backend, and assembler archives. `build` creates the pinned research environment and invokes `toolchain/build-legacy.sh` then `toolchain/build-assembler.sh`, retaining logs and a failure record. After these succeed, it packages installed artifacts into `/opt/pirates` in a Linux image and runs the compiler validator. A failed stage keeps validation false. A GitHub workflow runs the same experiment on native Linux for compiler-tooling PR changes. To retry only the frontend inside the research container after the bundled core is installed, pass `frontend` to `build-legacy.sh`.

The build script tests the frontend after LLVM installation, then builds the pinned Darwin ARM assembler. Its integration with the GCC driver still needs validation. A freestanding C/C++ smoke probe can establish the basic cross-build before any SDK-dependent game work. UIKit/Foundation/C++ SDK headers must be supplied locally when candidates require them.

## Validation and eventual runtime image

After a working frontend and assembler are available, `python tools/toolchain.py package` packages their installed artifacts and pinned host libraries into a Linux amd64 image and validates it. The compiler lives at `/opt/pirates`, outside the mounted workspace. The package includes pinned source archives with licenses, build scripts and host patches, package inventory, and hashes of installed files. It contains no SDK or game inputs.

`python tools/toolchain.py build` performs these stages together. The CI workflow caches source and build directories using source, dependency, and build-script hashes; make checks their dependencies. No runtime is marked valid merely because the backend or frontend compiled.

After probes succeed, `python tools/toolchain.py export` writes an ignored Docker archive plus its SHA-256 manifest. The native Linux workflow uploads this validated toolchain separately from diagnostics. This permits local `docker load -i runtime-image.tar.gz` on Linux or macOS without publishing a registry image. Verify the archive SHA-256 from `runtime-export.json`, install the supplied profile as `build/toolchain/compiler.json`, then rerun `tools/validate_compiler.py build/toolchain/compiler.json` on the receiving host before selecting it. Apple Silicon uses amd64 emulation.

The packager writes `build/toolchain/compiler.json` with an immutable local image ID. After successful validation, select it:

```sh
python configure.py --profile build/toolchain/compiler.json
python tools/dev.py doctor
```

The validator rejects Clang and plain GCC. It requires LLVM-GCC 4.2.1 / LLVM build 2336.9 identification, actual C and C++ compilation, ARM and Thumb Mach-O output, an included-header depfile, and identical object hashes from two build directories. A failed validator leaves validation false or absent. Its generated evidence is tied to the compiler image/binary fingerprint and invocation; source-build feasibility still does not prove byte-for-byte equivalence with the app’s historical compiler.

The container adapter mounts the project at `/work`, an optional local SDK at `/sdk`, and selects `linux/amd64`. It has been exercised with the validated historical image under Apple Silicon emulation. Validation evidence can be supplied to the CI runner with `--validation`. Native Linux CI validates the complete source recipe independently; SDK-dependent candidates still require a local SDK and remain untested here.

Apple’s GCC frontend is distributed under GPLv2-family terms; LLVM has its own open-source license and notices. SDK files have separate terms and are not bundled in this repository or its artifacts. No Apple account credentials are accessed by these tools.
