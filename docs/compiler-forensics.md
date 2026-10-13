# ARMv7 compiler forensics — 13 October 2026

**Decision: Outcome C, with a concrete replacement-backend hypothesis.** We have a usable research compiler, but we have **not identified or reproduced the compiler shipped in Xcode 4.3.2**. Its current bundled ARM backend is unsuitable as the sole basis for high-percentage exact matching of ordinary source. Exact source matching is demonstrably possible for selected functions, including all three problem instruction sequences with a nearby historical compiler and explicit source/flag hypotheses. High-percentage byte-perfect reconstruction remains unproven.

Do not expand assembly-transcription batches or treat their bytes as source recovery. Continue small, behavior-tested source reconstruction while making shipped-toolchain identification the next compiler milestone. Do not replace the production matching profile with the diagnostic Clang build yet.

## What is established

- The original executable still has SHA-256 `0e1f1ef80636463821d6431ece18da6b269b69d7e9e11991b73f654812fd3c9e`, 9,177 named STABS functions and 268 object groups. All named functions have ARM mode metadata. The original inventory and its lock were not changed.
- Apple's `llvmgcc42-2336.9/build_gcc` accepts an **external LLVM installation** as its ninth argument, uses it with `--enable-llvm`, and accepts submission-version arguments for `LLVM_VERSION_INFO`. Our recipe instead selects the embedded core and explicitly supplies `2336.9`. Its `llc --version` reports **2.7svn**. Matching frontend/version banners are not backend identity evidence.
- The current bundled `LowerCall` unconditionally disables ARM tail calls. Its Darwin global-address lowering always begins with a constant-pool load. Its UBFX selector accepts a shift-pair shape but lacks the newer shift-and-mask selection. Fresh object/assembly experiments confirm these differences; optimization, CPU, scheduling and PIC flags do not repair the first two lowering paths.
- Apple's published **clang-318.0.45** archive contains LLVM **3.1svn**, a Darwin MOVW/MOVT path, conditional ARM tail-call support and shift-and-mask UBFX selection. We built and tested it. This is a nearby public source revision, **not** Xcode 4.3.2's genuine compiler or proof of its expected clang-318.0.58 identity.
- In the fixed 43-function benchmark, current LLVM-GCC produces 14 exact relocated matches and the nearby Clang produces 15 at the existing unit profiles. Both reproduce the 136-byte `InitRenderStates` and 108-byte `PVRTMat3::RotationZ` functions, among other genuine source bodies across independent groups. Several matches are small controls; no larger animation routine matches exactly.
- The nearby Clang reproduces the matrix tail branch with deployment 5.1 or `-mllvm -arm-tail-calls=true`; the existing original-group NiObject RTTI candidate matches its MOVW/MOVT sequence; an unsigned 0/1 return hypothesis reproduces the alpha getter's LDR/UBFX/BX. These are complete relocation-aware byte matches, not operand masking or assembly payloads.

Confidence is **high** in the measured behavior and source differences, **medium** in a newer Apple LLVM backend as the right investigation direction, and **unresolved** for shipped compiler identity, original source spelling and high-percentage matching.

## Genuine Xcode retrieval and the precise blocker

The initial machine inspection found Xcode and Xcode-beta **27.0**, not 4.3.2. Bounded searches of Downloads, Documents, project resources, mounted volumes and Spotlight found no archived 4.3.2 installation/compiler. Existing untracked investigation files and old generated reports were audited rather than treated as fresh evidence.

The official [Apple download catalog](https://developer.apple.com/download/all/) redirects the collaborative browser to **Sign in to Apple Developer**. An unauthenticated request to [xcode_432_lion.dmg](https://download.developer.apple.com/Developer_Tools/xcode_4.3.2/xcode_432_lion.dmg) returns a 302 to `https://developer.apple.com/unauthorized/`. No credentials were accessed. A bounded archive search found other Xcode versions; a mirror's 1,890.14 MB 4.3.2 entry requires a donating account. Neither yielded compiler binaries. We did not bypass those requirements or substitute another distribution as genuine 4.3.2.

The missing artifact is **Xcode 4.3.2, build 4E2002**, distributed as `xcode_432_lion.dmg`, containing the **iPhoneOS** compiler/support files and SDK 5.1/9B176. An authenticated download or an independently preserved copy is needed. Keep the DMG/app outside Git. A community SHA-1 list is only a secondary consistency check, not Apple authentication; version strings and manifests alone cannot authenticate a copy.

Run on a supplied app without installing it or switching global Xcode:

```sh
python tools/inspect_xcode.py /absolute/path/Xcode.app \
  --run-tools --output build/compiler-forensics/xcode432-inventory
```

The inspector retains hashes, app/platform/SDK manifests, Mach-O load commands, architecture/dependency information, embedded build strings, signatures and verification failures. It inventories shared/desktop and iPhoneOS paths separately, retains aliases, includes `cc1*`, `libllvmgcc.dylib`, Clang, assembler and linker, and captures driver/specs/macros/help where runnable. `--library-path` records an explicitly supplied local runtime-library directory. Failures are preserved. If old binaries cannot run on this ARM64 host, use a compatible Intel macOS environment and the same CLI workflow; do not report them as tested here.

Create ignored profiles at the registry's `build/compiler-forensics/xcode432-llvmgcc.json` and `xcode432-clang.json` after inspecting the actual paths:

```json
{
  "name": "xcode432-iphoneos-llvmgcc-diagnostic",
  "family": "llvmgcc42",
  "command": ["/actual/inspected/iphoneos/llvm-g++"],
  "flags": ["-arch", "armv7", "-miphoneos-version-min=4.2"],
  "identity_files": ["/actual/driver", "/actual/cc1plus", "/actual/libllvmgcc.dylib"]
}
```

For Clang, use its inspected driver and native ARM selection options. These diagnostic profiles do **not** satisfy or bypass the production compiler validation gate. Genuine assembler/linker versions, default CPU, actual backend build, shipped configure choices and GCC support-file identity remain blocked, not inferred from our substitutes. The assembler currently used is a cctools-port hypothesis; the separate pinned ld64 is a structural-linking hypothesis. Function-object comparisons do not establish whole-image linker identity.

## Reproducible CLI and evidence model

The harness uses `recover`, `candidate_span` and `compare_function` from the existing inventory/matching core. It preserves original object-group associations, complete partial TUs, candidate padding and explicit symbol mappings. It resolves relocations at their actual sites; **no address or register masking, instruction reordering or original-byte source fallback** is introduced. Production source policy, compiler validation, candidate manifests and metrics are unchanged.

```sh
# Set up the existing reconstructed toolchain and local SDK as documented.
python tools/build_forensics_clang.py --jobs 8

python tools/compiler_forensics.py compare \
  --function f-8594ffda5a7ad6b10e37 --compilers all \
  --sdk build/sdk/iPhoneOS5.1.sdk --output build/compiler-forensics/single

python tools/compiler_forensics.py batch --sample config/compiler-probes.json \
  --compilers all --flag-search \
  --cases unit-profile,ios5.1,tail-enabled,portable-O2 \
  --sdk build/sdk/iPhoneOS5.1.sdk --output build/compiler-forensics/batch

python tools/compiler_forensics.py batch --compilers reconstructed --flag-search \
  --cases unit-profile,O1,O3,Os,no-postra,soft,align4 \
  --sdk build/sdk/iPhoneOS5.1.sdk --output build/compiler-forensics/flags

python tools/compiler_investigation.py --input research/Pirates-executable \
  --output build/compiler-forensics/reconstructed-probes-final
python tools/compiler_forensics.py probes --compilers apple-clang318-source \
  --output build/compiler-forensics/clang318-probes-final
python tools/compiler_forensics.py evidence --output build/compiler-forensics/original
python tools/compiler_forensics_audit.py

python tools/compiler_forensics.py batch --sample config/compiler-problem-variants.json \
  --compilers apple-clang318-source,reconstructed,modern-control --flag-search \
  --cases unit-profile,ios5.1,tail-enabled --sdk build/sdk/iPhoneOS5.1.sdk \
  --output build/compiler-forensics/problem-variants
python tools/compiler_forensics_execution.py
python tools/publish_compiler_forensics.py
```

`compare` also derives mappings for implemented candidates outside the fixed benchmark. To test new standalone hypotheses, supply an explicit sample. Registries can use existing container profiles, local historical drivers or wrapper commands and recorded environments. Separate library samples and per-unit flags allow group-specific compiler investigations. Missing genuine compilers remain explicit unavailable rows.

Each experiment retains exact argv, compiler identity/configuration, image/binary hashes, source/preprocessed/dependency hashes, flags, source-policy result, emitted assembly, object hash, sections/symbols/raw relocation metadata, fully resolved comparison events and readable Markdown. SDK content is fingerprinted. Errors, unsupported options, unresolved relocations and mode differences remain visible. No `matched` diagnostic row receives production progress credit. Runtime equivalence is `untested` unless a separate bounded execution report says otherwise.

The [committed evidence manifest](compiler-forensics/evidence/manifest.json) fingerprints compact JSON reports. Full assembly, objects, original instruction listings and binary inputs remain under ignored local paths. The publisher excludes original instruction bytes. [Readable per-function benchmark](compiler-forensics/benchmark.md), [benchmark JSON](compiler-forensics/evidence/benchmark.json), [flag results](compiler-forensics/evidence/flags.json), [problem hypotheses](compiler-forensics/evidence/problem-variants.json), [instruction census](compiler-forensics/evidence/original-census.json), [all object-group hypotheses](compiler-forensics/evidence/object-group-hypotheses.json) and [source audit](compiler-forensics/evidence/source-audit.json) are reviewable without proprietary binaries.

### Nearby historical Clang build limitations and repairs

The public archive and commit/SHA-256 are pinned in `toolchain/compiler-forensics-sources.lock.json`. The build uses its included backend, ARM-only targets, integrated assembler, an explicit `-ccc-host-triple armv7-apple-darwin11`, and copied Clang resource headers. This driver predates the modern `-target` interface; supplying `-target armv7-apple-ios4.2` was rejected as an input filename, then corrected. Default host-target banners are not ARM invocation evidence; `-###` records actual CPU/ABI/mode.

Host repairs are explicit in `tools/build_forensics_clang.py`: Python 2 build tooling is converted once with Python 3's `lib2to3`; unrelated libc++ project discovery is excluded; dependent-template qualifiers, stale ASTVector member names/argument order and iterator value-type declarations are repaired. **The ASTVector repairs touch frontend implementation**, so this port is not a pristine shipped compiler. No ARM target/code-generation source file is patched.

The initial optimized host build crashed in `SSAUpdater::~SSAUpdater` during Jump Threading and Tail Duplication. Its source deletes through a reference formed from a possibly null opaque pointer; modern host optimization can remove the historical null check. Building with `-fno-delete-null-pointer-checks -fno-strict-aliasing` repaired the observed crashes. The final benchmark compiles all 43 functions with this historical candidate, including animation TUs. These are host-compatibility settings, not target flags. The final three-function matches and bounded execution were repeated after this rebuild.

This diagnostic build must not be installed as the authoritative matching compiler merely because it succeeds. Source recipe, repairs, binary hashes and actual invocation are the identity evidence.

## Experiment A — tail calls

Original `f-8594ffda5a7ad6b10e37`, `PVRTMatrixMultiplyF`, is 20 ARM bytes: four register moves then a direct branch to the known ARM `NEON_Matrix4Mul` definition. The genuine-source candidate calls that named function with `(a.f, b.f, out.f)`.

**Confirmed:** current bundled ARM `LowerCall` sets `isTailCall = false` unconditionally. O0/O1/O2/O3/Os, explicit sibling calls, LLVM tailcall option, ARM/Thumb, CPU tuning and Darwin triple experiments do not reproduce the original tail transfer. Its tested O2 candidate is 32 bytes with a frame and BL. This limitation is stronger than a failed speculative source candidate because it is enforced in backend source.

**Confirmed:** nearby Apple Clang 318.0.45 compiles the **same existing source TU** to a complete exact 20-byte match with deployment 5.1 or `-mllvm -arm-tail-calls=true`. At deployment 4.2 it emits a framed call. Its subtarget enables default tail calls only for iOS >=5, and LowerCall permits the explicit override. Minimal integer/void probes reproduce that distinction. ABI probes remain separate; changing to a different target/ABI is not accepted as an original-game match.

**Unresolved:** whether the PowerVR archive used this Clang, a compatible shipped LLVM-GCC backend, a different deployment profile, custom patches or assembly. The app's Mach-O minimum OS 4.2 does not prove every static-library compile used 4.2. Enabling tails globally reduces benchmark exact matches, so uniform tail flags are not justified.

## Experiment B — global address materialization

Original `f-5a027fbd3240a6a0e288`, `NiObject::GetRTTI`, is MOVW/MOVT/ADD-PC/BX, 16 bytes, returning the original named `__ZN8NiObject6m_RTTIE` allocation.

**Confirmed:** current bundled Darwin lowering uses a pool for ordinary global addresses. Its `arm-use-movt` option affects the ELF lowering path and does not introduce a Darwin MOVW/MOVT path. PIC/non-PIC, visibility, definition/declaration, CPU and Darwin triple experiments leave that structural limitation.

**Confirmed:** the nearby Clang's `LowerGlobalAddressDarwin` uses WrapperPIC/WrapperDYN when MOVT is enabled, and its `arm-darwin-use-movt` default is true for capable CPUs. Turning that flag off restores pool addressing. Its **existing original-group `TempIncludeCpp2.cpp` candidate** and the separately declared same-unit RTTI probe reproduce the full original bytes with proven symbol/subtractor relocations.

A source audit caught an error in the pre-existing untracked minimal probe: it declared `ms_RTTI`, whereas the original symbol is **`m_RTTI`**. It also lacked the original same-unit definition, causing non-lazy-pointer lowering. Those source/context errors are corrected in the diagnostic probes and explicitly distinguished from backend capability. The benchmark uses the existing partial unity TU, not that isolated declaration. RTTI object initialization, class hierarchy and complete metadata remain unproven; the match establishes this address-return function only.

**Unresolved:** which shipped GCC/LLVM backend or frontend produced the original getter. Matching a nearby Clang's code is compatibility evidence, not proof that Gamebryo was compiled with Clang.

## Experiment C — UBFX versus shift-and-mask

Original `f-4d69d9f014efa7e4d2e9`, `ISETexture::IsAlphaEnabled`, is LDR `[this+0x2c]`, UBFX bit 15 width 1, BX, 12 bytes.

**Confirmed:** the current ordinary bool-return source produces shift/mask code; optimized signedness/CPU/shift-pair probes do not repair the selector for the canonical register shift-and-mask shape. **Contradicted:** “this backend cannot emit UBFX.” An O0 shift-pair probe does emit UBFX. The instruction exists; IR/DAG shape and optimization matter.

**Confirmed:** nearby Clang's selector recognizes `(x >> n) & low_mask` directly and emits UBFX for unsigned/signed integer probes. The bool-return getter still differs: the frontend lowers it to a boolean test and the backend can narrow its load. Making flags volatile forces a full load but still emits AND/LSR for bool. **Volatile did not solve the discrepancy** and would change observable memory semantics.

**Confirmed under an explicit source hypothesis:** `unsigned IsAlphaEnabled() { return (flags >> 15) & 1; }` with ordinary nonvolatile flags reproduces all 12 original bytes under the nearby Clang. Both the return type and volatility are absent from the retained C++ symbol/STABS map. The original return declaration is therefore **unresolved**, not assumed to be bool or inferred as unsigned from this one match. The existing production candidate is unchanged.

The [bounded execution report](compiler-forensics/evidence/execution.json) passes 321 alpha flag scenarios, checks the RTTI pointer, and checks matrix caller argument permutation/stack return through a modeled callee. This is separate runtime evidence for those observations. It does not validate matrix arithmetic, RTTI contents, lifetimes, allocation, complete animation behavior or native iOS execution.

## Compiler/flag compatibility matrix

The detailed matrix is in the JSON reports; these are complete comparisons, not best-of-many percentages.

| Compiler / profile | Exact / 43 | Different | Unresolved | Compile errors | Interpretation |
|---|---:|---:|---:|---:|---|
| Current LLVM-GCC, existing unit flags | 14 | 26 | 3 | 0 | Useful partial source matching; missing newer ordinary patterns |
| Nearby Apple Clang 318.0.45, existing unit flags | 15 | 24 | 4 | 0 | Pattern-compatible, shipped identity unproven |
| Nearby Clang, deployment 5.1 | 11 | 28 | 4 | 0 | Fixes matrix tail; worsens other groups |
| Nearby Clang, explicit ARM tails at 4.2 | 11 | 28 | 4 | 0 | Same tail effect; group-specific flag needed |
| Modern Apple Clang control, existing unit flags | 6 | 27 | 5 | 5 | Unsupported `-fno-toplevel-reorder`; not historical identity evidence |
| Modern control, explicit portability case | 6 | 32 | 5 | 0 | Removes only that unsupported flag, records removal |
| Genuine Xcode 4.3.2 LLVM-GCC / Clang | unavailable | — | — | — | Authentication/runtime prerequisites unresolved |

O1, O3, Os, software float, post-RA scheduling and function alignment were bounded searches. O3/Os do not improve the current exact count; disabling post-RA scheduling globally worsens it, despite its existing benefit for FPhono wrappers. Software float creates additional unresolved soft-float helper references in arithmetic units; it is not a universal profile. Alignment 4 does not repair the three patterns. Unsupported options remain failures, including newer backend options on the 2.7 core. Equal status counts do not imply equal object hashes; inspect individual rows and retained assembly.

## Representative sample and uncertainty

The [fixed sample](../config/compiler-probes.json) contains **43 functions, 19,352 original bytes, 14 independent groups**. It includes previously exact C++, nonmatching matrix/vector operations, conditional bounds/plane/camera code, calls/global relocations, a linked-list animation-controller accessor, and larger ControllerSequence destruction/load routines (up to 4,976 original bytes). Two empty Objective-C methods and small copy/hash controls are explicitly visible; they do not dominate the sample.

Each compiler receives the same source file and its complete current partial TU. Unit-profile, deployment and tail cases are reported separately. `portable-O2` records the removed `-fno-toplevel-reorder`; no source is rewritten for a particular compiler. Runtime behavior is untested for the 43-function benchmark except where a separate, explicitly different three-function hypothesis report applies.

Large animation sources are incomplete and speculative. `GetNodeMatrix`/dispatcher use ABI register bindings; `CreateSequenceFromFile` has an uninitialized size argument. These remain compiler comparisons, not behavioral proofs. Unresolved animation relocations include unsupported SjLj/personality/local-data placement dependencies. No original callee fallback or relocation masking turns them into matches. The sample does not establish universal compiler identity or estimate a game-wide achievable percentage.

One draft sample entry had no implementation (`NiTriShapeData::GetTriangleIndices`). It was replaced after a missing-source audit with the genuine `NiTimeController::ItemsInList` linked-list candidate; no output-guided source change was made. NiObject uses its original-group partial unity TU after correcting the isolated probe's symbol/context mistake.

## Original groups and build settings

There are no retained N_OPT compiler-producer records or compiler marker symbols in the parsed symbol table. N_SO/N_SOL paths, N_OSO ownership/timestamps and function names identify source/object provenance, not a unique compiler. Object timestamps point to the May 2, 2012 build, but are not compiler-version certificates. IPA plist compiler/Xcode/SDK metadata is the app-target hypothesis, not a record for every archive member. The archive hash identifies the supplied archival input, not an authenticated Apple CDN executable.

A conservative census finds the **adjacent MOVW/MOVT + PC-relative ADD/LDR signature in 2,423 functions across 209 groups**. This is a positive addressing signature, not a count of functions that can never match the current compiler. Literal-pool/data-in-code heuristics and functions with unusual control flow limit disassembly inference. It nevertheless shows the global-address discrepancy is widespread across app objects as well as libraries, not merely one getter.

The bounded terminal-branch detector finds **10 terminal unconditional branches to known outside definitions**, all in four PowerVR archive groups (`PVRTString`, `PVRTResourceFile`, `PVRTMatrixF`, `PVRTModelPOD`); none in app-object groups under this detector. This supports **investigating different library profiles**. It does not establish all tail transfers, compiler families, original source or assembly absence.

| Original family / group | Evidence | Working hypothesis / remaining uncertainty |
|---|---|---|
| Game-specific C++ unity | ARM metadata; widespread address pairs; tiny/hash candidates match multiple compilers | App LLVM-GCC metadata is relevant; newer compatible backend plausible. Nontrivial whole-group identity unproven |
| Gamebryo contributed unity, especially TempIncludeCpp2/4 | Named RTTI data, direct getters; nearby Clang reproduces address sequence | Definition visibility/ownership matter. GCC versus Clang frontend and static initialization remain unknown |
| PowerVR OGLES2Tools | Source math/copies match both research compilers; terminal branches in four groups | Investigate tails enabled separately / library deployment >=5 / compatible backend; original NEON assembly may exist |
| ISE library | UBFX getter, address pairs, render-state source match, nontrivial bounds/camera differences | Return types, float conventions, layout and source semantics must be audited before attributing failures to compiler |
| Audio / Phono2 / FPhono | ARM; ordinary call wrappers, recorded soft/scheduler profile, limited matches | Current wrapper matches are useful; do not generalize scheduling or float settings to all audio |
| TinyXML, standalone and ISE archive copies | Different original object groups and address-pair prevalence | No trustworthy nontrivial TinyXML source benchmark here; compiler families/settings unresolved |
| Objective-C application | Method symbols/runtime evidence and app metadata; two empty methods match | Insufficient body complexity and unrecovered class metadata for compiler attribution |
| Other and groups with no functions | Original inventory retained | Unresolved; census is not a compiler identification |

Every one of the 268 groups has an explicit [hypothesis record](compiler-forensics/evidence/object-group-hypotheses.json), source-path/timestamp/mode evidence, sample coverage and confidence. No group is silently assigned a compiler based on names alone. Default CPU, visibility, exception/RTTI settings, ABI, alignment and frontend macros are invocation evidence for tested candidates, not recovered original settings. All source files were screened for compiler-conditioned implementations; the benchmark source has no `__clang__`/`__GNUC__` implementation split.

## PR #10 and source-integrity audit

PR #10 claimed +1,031 exact matches and 956,652 total matched bytes. Its source introduced large inline assembly/`.word` bodies. Those are useful transcription artifacts but **cannot establish recovered C++ or compiler fidelity**.

The read-only Git audit resolves the requested October 9 shorthand to **`c18b410679cdd02187a419a1179b608e1f6581fe`** (the supplied full hash omitted one `d`). It audits that commit, PR #10's parent, PR #10 (`ff8e9be6e48eeaaa497ec29209b9ac31b2a2fd46`) and the working source baseline (`16022a0`).

- The pre-PR #10 snapshot already has **7 candidate TUs with prohibited assembly**, affecting 28 implemented records. It is not a clean high-level benchmark by default.
- PR #10 adds **1,002 manifest function IDs**, all in TUs rejected for assembly payloads. Its complete manifest has 17 affected TUs / 1,211 implemented records subject to whole-TU rejection. These are conservative **TU source-form counts**, not a reproduction of the PR's +1,031 byte-match claim or a count of individual assembly bodies. Some pre-existing entries were also changed, explaining why manifest additions and claimed new matches are different measures.
- The current baseline's literal-include screen finds no prohibited payload TU among configured candidates. An EAGLView declaration-label macro needs preprocessing; actual historical preprocessing accepts it. Unselected legacy assembly files remain preserved in Git.

The current policy is version 3 and runs on the **matching compiler's preprocessed TU**, including forced/local includes and expanded macros. It rejects assembly bodies, `.word`/`.byte`/`.incbin` and explicit instruction-section data while allowing ABI labels/register bindings and ordinary data. Stale objects lacking current policy validation are rejected; failed compilation deletes stale output. The forensic harness enforces the same policy and never promotes diagnostic output into production metrics.

The historical PR #10 build engine had no `validate_source` call, so its assembly credit did not bypass an existing current gate: it used an earlier engine without that gate. Current repository tests exercise include/macro payloads, forged system paths, declaration labels and executable-section encodings. No current source-policy bypass was found. This is a strong form check, **not a proof of original semantics or protection against every conceivable disguised code generator**. Arrays, register-bound reconstructions and synthetic ABI layouts still need human semantic review. Preserve source recovery, transcription, exact bytes, fuzzy similarity and bounded runtime as separate measures.

## Recommendation and next decisive action

**Do we have the correct compiler?** Not established. The current source recipe's backend selection is unsupported as shipped identity, and its ordinary ARM patterns differ in ways flags cannot repair.

**What have we proven?** Fresh relocation-aware high-level comparisons, cross-group source matches, complete reproduction of the three problem sequences by a nearby historical backend under explicit hypotheses, and narrowly bounded execution. A visible version number and many tiny matches prove neither identity nor high coverage.

**Can it reproduce nontrivial original ARMv7 instructions?** Yes for selected genuine source functions; the nearby historical candidate also reproduces the formerly missing addressing/tail/bit-extract patterns. Current larger animation reconstructions do not match exactly and have unresolved semantics/relocations.

**Which groups remain uncertain?** All groups lack definitive compiler identity. Gamebryo/app unity, PowerVR deployment/tails, ISE return/float/layout choices, audio profiles, both TinyXML copies and Objective-C metadata need separate evidence. The per-group report preserves that uncertainty.

**Is source decompilation viable?** Yes as a behavior-led reconstruction process and for selected exact functions. High-percentage byte-perfect matching is **not yet technically justified as the primary progress target**. This investigation is neither proof of impossibility nor Outcome A. Outcome B requires a comparison to the genuine distributed compiler that we do not yet have.

**Next action with greatest value:** obtain/run the genuine iPhoneOS Xcode 4.3.2 LLVM-GCC and support dylib, fingerprint them, then run this fixed 43-function sample and three problem variants without changing source. If its backend differs, identify the matching backend/bridge recipe and repeat with per-library profiles. A nearby source port is a useful compatibility comparator, not the final replacement.

Until that artifact is available, keep exact byte matching as a secondary measured outcome; prioritize meaningful high-level semantics and connected runtime tests, keeping future ARM64 portability in view. Avoid spending large recovery batches tuning against the known-incomplete 2.7 Darwin lowering. Do not add target patches merely to synthesize the desired three patterns: a patched nonhistorical compiler would need independent validation and would not identify the original toolchain.

## Validation and practical limits

All **145 repository unit tests** pass (`python -m unittest discover -s tests`), including the three new forensic integration tests. All **323 bounded execution scenarios** pass in the separate problem-hypothesis report. The new tests check included macro assembly rejection/stale-object deletion, original fingerprint/object-group enforcement, explicit unavailable compilers and dependency-sensitive comparisons. Current compiler O2 probe object hashes repeat identically. The final source Clang builds all benchmark TUs after host fixes. Controlled newer options on the old core and `-fno-toplevel-reorder` on modern Clang remain disclosed failures, not hidden substitutions.

No genuine Xcode binary experiment or full-game replacement link/runtime was performed. No compiler universal-identity claim, animation behavioral proof or whole-image byte equality is made. Proprietary game inputs, SDK contents and Apple compiler binaries are not committed. The matching engine, original input inventory and game candidate source were not modified.

### Primary source references

- [Apple LLVM-GCC 2336.9 build script](https://github.com/apple-oss-distributions/llvmgcc42/blob/c92700f7f0438a4bd5084145b9f351de63256e66/build_gcc): external backend and version injection.
- [Bundled ARM lowering](https://github.com/apple-oss-distributions/llvmgcc42/blob/c92700f7f0438a4bd5084145b9f351de63256e66/llvmCore/lib/Target/ARM/ARMISelLowering.cpp) and [selector](https://github.com/apple-oss-distributions/llvmgcc42/blob/c92700f7f0438a4bd5084145b9f351de63256e66/llvmCore/lib/Target/ARM/ARMISelDAGToDAG.cpp): current limitations.
- [Apple Clang 318.0.45 ARM lowering](https://github.com/apple-oss-distributions/clang/blob/7731212dc84f391254f376e8f03753437b83f242/src/lib/Target/ARM/ARMISelLowering.cpp), [subtarget](https://github.com/apple-oss-distributions/clang/blob/7731212dc84f391254f376e8f03753437b83f242/src/lib/Target/ARM/ARMSubtarget.cpp) and [selector](https://github.com/apple-oss-distributions/clang/blob/7731212dc84f391254f376e8f03753437b83f242/src/lib/Target/ARM/ARMISelDAGToDAG.cpp): tested newer paths and deployment-conditioned tails.
- [Apple compiler overview](https://developer.apple.com/library/archive/documentation/CompilerTools/Conceptual/LLVMCompilerOverview/): the distinction between GCC/Clang frontends and LLVM code generation; no binary-equivalence conclusion follows from sharing LLVM.
