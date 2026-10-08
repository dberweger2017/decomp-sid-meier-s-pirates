# Historical compiler feasibility

The LLVM-GCC hypothesis now has a **validated C, C++, Objective-C and Objective-C++ ARMv7 cross-build**, tested on native Linux CI and in a pinned Linux amd64 container under Apple Silicon emulation. ARM and Thumb Mach-O probes, included-header dependencies, repeated object hashes, and the historical Ninja match/header-regression loop pass. Exact equivalence to the app's shipped compiler remains unproven. `config/compiler.json` is a template with a null image; `tools/toolchain.py build` generates a locally pinned, validated profile in `build/toolchain/compiler.json`. No modern compiler substitutes for matching compilation.

## Evidence

The archive’s Info.plist names LLVM-GCC 4.2, Xcode 4.3.2 (4E2002), and iOS SDK 5.1 (9B176). This identifies the app target’s compiler family. It does not prove the compiler patch, backend revision, per-unit flags, or compilers used for linked static libraries.

The frontend experiment starts with Apple’s [llvmgcc42-2336.9 source](https://github.com/apple-oss-distributions/llvmgcc42/tree/llvmgcc42-2336.9), commit `c92700f7f0438a4bd5084145b9f351de63256e66`, and now uses the LLVM core bundled in that archive. The separately published [llvmCore-2326.12](https://github.com/apple-oss-distributions/llvmCore/tree/llvmCore-2326.12), commit `a250b96ad7af34aa6099535d78dc0a71d57d856e`, built but failed frontend integration: it lacks `GlobalValue::LinkerPrivateWeakLinkage` and `Type::getIntNTy`. The bundled core contains both required APIs. Its relationship to the shipped Xcode backend has not been established; source-build feasibility does not prove exact compiler equivalence.

The experiment ran Linux amd64 under Docker on a macOS arm64 host. The base image is pinned by digest; the installed package inventory is locked and checked. Compiler source archives are pinned by commit and SHA-256 in `toolchain/sources.lock.json`.

[Native Linux validation run](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37693756924) passed the complete source recipe, runtime probes, image export, and historical incremental regression smoke test. The exported CI archive was SHA-256 verified and imported on macOS ARM64; receiving-host C/C++ ARM/Thumb object hashes equal native CI’s hashes. The expanded four-language SDK validation also passed [native Linux CI](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37735419198). Recorded stages in `toolchain/feasibility.json`:

1. CMake failed on stale source lists (`ModuleProvider.cpp`) and an absent test directory. Switched to the documented configure/make route.
2. GCC 5 exposed missing host includes for `ptrdiff_t` and `lseek64`. Explicit host include flags fixed these errors. The archived module-link rule used `-module`; a recorded host-only change uses Linux `-shared` instead.
3. The LLVM ARM backend, libraries, `llc`, `llvm-config`, and unit-test binaries built. `install-libs` avoids documentation generation requiring `groff`.
4. One attempt ended with Docker’s `error waiting for container: unexpected EOF`, followed by disappearance of the daemon socket. The user reported closing Docker; after restarting it, the cached build resumed.
5. The GCC frontend's bridge ignores configured CXXFLAGS. Passing host C++ includes through make's CXX command repairs missing `ptrdiff_t`; `-fpermissive` permits old constructor expressions in the never-called library link helper. Missing flex/bison inputs require pinned parser generators. The frontend subsequently passed compilation and probes using the bundled backend.
6. Source inspection confirms assembly-file emission. The pinned Linux port of cctools 845's ARM GAS built with GCC host compilation, explicit port visibility definitions and little-endian host flags. An independent Thumb-2 probe produced a valid ARMv7 Mach-O object with the expected Thumb symbol. Its revision is an assembler hypothesis, not evidence of the assembler shipped with Xcode 4.3.2.
7. The frontend reached compilation of its LLVM bridge and rejected the separate backend's missing APIs. The build now selects the frontend archive's bundled core, without target-code patches.
8. Native Linux built the bundled core but exposed omitted tool installation; `llc` and `llvm-config` are now installed explicitly. The resumed frontend on Linux exposed an old `mempcpy` declaration conflicting with glibc. A host-only `__GLIBC__` guard uses the system declaration without changing helper logic or target generation.
9. C/C++ frontends and drivers built and installed. Default `all-gcc` also attempts target libgcc and Darwin crt3, which require SDK headers. Compiler-only make flags omit those runtime/linking artifacts. The compiler recipe validates `-c` object compilation and provides no target runtime. The separate linker recipe uses SDK libraries and validates structural ARMv7 linking; the full game remains incomplete. SDK files are mounted separately. The current recipe also enables Objective-C and Objective-C++; both frontends pass repeated ARM/Thumb object probes.
10. The container passed C/C++ ARM and Thumb Mach-O, header-dependency, and repeated-object probes. Its compiler source's plain `armv7` architecture omits ARM mode capability (`FL_NOTM`); explicit `armv7-a` works. This is an investigation flag, not a recovered original flag. The historical Ninja loop verified a synthetic four-byte function, rebuilt on a header edit, and detected its lost match.

The scripts preserve these host portability changes. They do not modify ARM code generation. Build logs are in the ignored local `build/toolchain/` cache; concise evidence is tracked separately. Do not infer feasibility of the full cross-build from the LLVM core build alone.

## Reproduce the investigation

```sh
python tools/toolchain.py fetch
python tools/toolchain.py build
python tools/toolchain.py status
```

`fetch` verifies the frontend, backend, and assembler archives. `build` creates the pinned research environment and builds the assembler and invokes `toolchain/build-legacy.sh`, retaining logs and a failure record. After these succeed, it packages installed artifacts into `/opt/pirates` in a Linux image and runs the compiler validator. A failed stage keeps validation false. A GitHub workflow runs the same experiment on native Linux for compiler-tooling PR changes. To retry only the frontend inside the research container after the bundled core is installed, pass `frontend` to `build-legacy.sh`.

The assembler integrates with the GCC driver through installed relative symlinks. Freestanding probes establish all four language frontends. `python tools/sdk.py fetch` supplies the pinned SDK described in [SDK setup](sdk.md). `python tools/validate_sdk.py --profile build/toolchain/compiler.json --sdk build/sdk/iPhoneOS5.1.sdk` then verifies stdlib/OpenGLES, GNU C++ headers, Foundation Objective-C, and Foundation/C++ Objective-C++ probes in ARM and Thumb modes.

## Validation and runtime image

After the frontend and assembler build, `python tools/toolchain.py package` packages their installed artifacts and pinned host libraries into a Linux amd64 image and validates it. The compiler lives at `/opt/pirates`, outside the mounted workspace. The package includes pinned source archives with licenses, build scripts and host patches, package inventory, and hashes of installed files. It contains no SDK or game inputs.

`python tools/toolchain.py build` performs these stages together. The CI workflow caches source and build directories using source, dependency, and build-script hashes; make checks their dependencies. No runtime is marked valid merely because the backend or frontend compiled.

After probes succeed, `python tools/toolchain.py export` writes an ignored Docker archive plus its SHA-256 manifest. The native Linux workflow uploads this validated toolchain separately from diagnostics. Download and extract that artifact, then import it on Linux or macOS without publishing a registry image:

```sh
python tools/toolchain.py import --artifact /path/to/extracted-artifact
python configure.py --profile build/toolchain/compiler.json
```

Import verifies the archive SHA-256, source lock, platform and installed-artifact manifest. Docker versions may assign different image IDs when loading an archive; import pins the receiving host’s immutable ID, reruns the compiler probes, and requires their hashes to equal CI’s hashes. Apple Silicon uses amd64 emulation.

The packager writes `build/toolchain/compiler.json` with an immutable local image ID. After successful validation, select it:

```sh
python configure.py --profile build/toolchain/compiler.json
python tools/dev.py doctor
```

The validator rejects Clang and plain GCC. It requires LLVM-GCC 4.2.1 / LLVM build 2336.9 identification, actual compilation of every configured language, ARM and Thumb Mach-O output, an included-header depfile, and identical object hashes from two build directories. A failed validator leaves validation false or absent. Its generated evidence is tied to the compiler image/binary fingerprint and invocation; source-build feasibility still does not prove byte-for-byte equivalence with the app’s historical compiler.

The container adapter mounts the project at `/work`, an optional local SDK at `/sdk`, and selects `linux/amd64`. It has been exercised with the validated historical image under Apple Silicon emulation. Validation evidence can be supplied to the CI runner with `--validation`. Native Linux CI validates the complete source recipe and all four SDK-dependent language probes independently. The same probes pass under Apple Silicon emulation. Sixteen actual C++ source functions verify at 628 bytes; see [candidate evidence](easy-candidates.md).

Apple’s GCC frontend is distributed under GPLv2-family terms; LLVM has its own open-source license and notices. SDK files have separate terms and are not bundled in this repository or its artifacts. No Apple account credentials are accessed by these tools.

## Observed matching limitations

The bundled core used by `toolchain/build-legacy.sh` explicitly sets
`isTailCall = false` in ARM `LowerCall` (`llvmCore/lib/Target/ARM/ARMISelLowering.cpp`
in the pinned frontend archive). The straightforward PVRTMatrixMultiplyF source
therefore produces a BL with a frame rather than the original function's B tail
branch. This is a concrete backend limitation of the current validated profile,
not evidence that all original object groups used this backend or compiler.
The shipped backend and compilers for linked libraries remain unidentified;
existing exact matches do not establish universal compiler equivalence.

For FPhono, disabling post-register-allocation scheduling verifies sixteen ordinary
call wrappers including their resolved branch targets. The same setting leaves
SetDopplerFactor's MOVT/register-move ordering different. Per-unit investigation
flags and nonexact candidates stay visible; no instruction reordering is masked
and no modern compiler or assembly fallback receives real-game source credit.

The same bundled core's `LowerGlobalAddressDarwin` always materializes global
addresses through a constant-pool load. `-arm-use-movt` controls the ELF lowering
path, whose MOVW/MOVT route is absent from this Darwin implementation. Isolated
NiObject::GetRTTI probes with ordinary and hidden declarations both produce
literal-pool loads, while the original getter uses MOVW/MOVT plus PC-relative
address arithmetic. The attempted non-lazy-pointer placement also remains
unresolved. These getters are deferred rather than assigned match credit. This
second concrete backend discrepancy reinforces the need to identify the shipped
backend before claiming all original code can be reproduced with this profile.
