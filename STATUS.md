# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026, approximately 15:07 CEST (Europe/Zurich)  
**Repository:** [dberweger2017/decomp-sid-meier-s-pirates](https://github.com/dberweger2017/decomp-sid-meier-s-pirates)  
**Merged source baseline:** PR #2, commit [`920d7daea53`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/920d7daea53f98988c0c8dc23fb345d72a0af1d9); later main commits have only updated `STATUS.md`.  
**Active draft:** [PR #3 — Recover ARMv7 functions in verified batches](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3), head [`1f25b660eb1`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/1f25b660eb1b145809fe9ed72ff495d0c4644bbd).  
**Verification:** [Latest branch CI](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37781126678) **passed** on macOS, Ubuntu and historical candidate progress; the recorded job log reports 411 exact functions / 4,864 code bytes and a passing deliberate regression/recovery test.

> **Executive summary:** The ARMv7 compiler, original-byte matcher, SDK and historical linker workflows are functioning for supported cases. Draft PR #3 has reached **411 exact functions / 4,864 code bytes**, with an imported-call relocation improvement, **67 configured original groups**, one 4-byte matched data allocation and structurally linked diagnostic subsets. Most original code remains unrecovered. There is **no runnable complete game** and **no arm64 iPhone port**.

## Exact matching progress

| Measure | Merged `main` | Draft PR #3 |
|---|---:|---:|
| Original named STABS functions | 9,177 | 9,177 |
| Original object groups in executable | 268 | 268 |
| Original inventoried function bytes | 4,080,584 | 4,080,584 |
| Fully byte-matched functions | **16** | **411** |
| Fully byte-matched function bytes | **628** | **4,864** |
| Function-record match ratio | 0.1743% | **4.4786%** |
| Original function-byte match ratio | 0.0154% | **0.1192%** |
| Configured original source groups | 5 | **67** |
| Differing compiled candidates | 5 | **34** |
| Function records without source | 9,156 | **8,732** |
| Original non-code data allocations | 5,056 / 3,827,236 bytes | Same |
| Fully matched non-code data | 0 bytes | **4 bytes (one allocation)** |
| Completed replacement-link units | **0** | **0** |
| Playable original replacement / modern arm64 build | No | No |

**Delta since the preceding 236-function report:** **+175 functions and +1,964 exact code bytes**. **Delta versus merged main:** +395 functions and +4,236 exact code bytes.

Coverage of **functions by count** is not coverage of **machine-code bytes**, which is much lower because many recovered routines are small. Neither metric estimates percentage of total decompilation work, nor the time to a playable build. The exact-data allocation is counted separately from function bytes.

## Milestones in the active recovery PR

### Batch 1: all 100 exact

- **100 / 100 verified functions; 660 bytes**.
- Five original unity compilation groups: `FireIncludeCpp.o`, `FireIncludeCpp2.o`, `PiratesIncludeCpp2.o`, `PiratesIncludeCpp3.o`, `PiratesIncludeCpp4.o`.
- Initial Pirates-specific UI/world, engine and Gamebryo accessor work: field reads, indexed child/animation access, state resets, observed constant returns and release callbacks.
- A UicButton included-header edit was confirmed to rebuild only the affected original unity object while preserving earlier matches.

### Batch 2: 98 exact of 100 examined

- **98 / 100 verified functions; 904 bytes** spread across 44 original groups.
- Work includes engine/game unity code, multiplayer UI, ISE graphics/particle systems, Phono2 audio and various field, mesh, vertex, emitter and pointer accessors.
- The two nonmatching source bodies remain visible as **different**, not incorrectly counted as recovered exact code. The resulting 334-function checkpoint passed its GitHub Actions jobs.

### Batch 3: 77 exact of 102 examined

- **77 / 102 verified functions; 1,060 bytes** across 18 original groups.
- Resulting project checkpoint: **411 exact functions / 4,864 exact code bytes**.
- Partial `NiTMapBase`/`NiTMapItem` C++ templates now support some hash/key/value routines, pointer identity checks and observed empty hooks. Other reconstructed operations include vector/RTTI initialization, particle count clamping, emitter controls and packed-version construction.
- This batch also demonstrates **real imported ARM-call stub resolution** with a working unsigned-remainder call. The matching engine checks `LC_DYSYMTAB` indirect-symbol records, a supported 12-byte stub encoding and associated lazy-pointer entries. Wrong/ambiguous/imported-pointer cases remain conservatively unresolved.
- **25 of the third-batch candidates** remained nonexact, including 22 map equality helpers and two audio validity methods at ~75% assembly similarity and an accumulator start method. No false exact-credit is assigned. Total nonexact candidates across the PR: **34**.
- **Seven new synthetic tests** cover import-stub resolution and negative cases.

## What source areas have we matched?

- **PowerVR platform and math:** shell callbacks; matrices, vectors, quaternions, strings and resource-file accessors.
- **Fireplace audio:** GameAudio, `FSound`, `FSound3D`, `FAudioManager`, `FAudioSystem`, `FSharedSoundData`, `FPhono`, `FKnob`, `FSoundScape` and other small audio methods.
- **Engine/graphics support:** ISE particle systems, buffers/vertex/mesh structures, texture/camera flags, emitters, Gamebryo map helpers, partial RTTI and type access.
- **Pirates UI/world glue:** partial unity-source definitions, indexed UI/animation state, menu/dance/multiplayer scene accessors and world-object fields.
- **Global data:** four bytes for the PowerVR `CPVRTString::npos` sentinel, placed in the observed original section/alignment.

**Important scope limit:** Names and partial declarations of Pirates gameplay/UI classes do **not** amount to recovered sailing, ship combat, swords, economy, quests or coastal-bombardment systems. Many partial class/virtual layouts cannot safely be instantiated.

## Tooling and reproducibility status

**Merged foundations (PR #1 + #2)**

- Original 32-bit iOS v1.1.2 executable imported from a hash-pinned archival IPA. Its identity is verified against the recorded archive, **not Apple-authenticated source provenance**.
- `9,177` STABS function records, `268` original object groups, deterministic inventory, original ARM/Thumb modes and preserved unity units.
- Historical LLVM-GCC 4.2.1 / LLVM build 2336.9 ARM Mach-O compiler, assembler and a content-pinned iPhoneOS 5.1 SDK; C, C++, Objective-C and Objective-C++ compile probes.
- Pinned historical ld64 linker; repeatable structural SDK link probes, not proof of original game's startup.
- Browser-based disassembly/diffs, source/header watching, Ninja incremental rebuilds, exact relocated-byte verification, distinct fuzzy similarity and progress reports, and regression-protected CI.
- A separate inventory of 5,056 non-code allocations including BSS, alignment/padding, constants, pointers, vtables and Objective-C metadata.

**New on PR #3**

- The recovery loop batches approximately 100 related definitions without changing original compile-group boundaries.
- Original relocations of uniquely resolved imported ARM call stubs can be checked against their indirect symbol and lazy-pointer identities.
- The **62-group diagnostic link** structurally links **380 exactly matched functions / 3,780 code bytes**, **27 differing candidates**, and the matched npos sentinel. Image SHA-256: `a1503e8ff803e5a97c588c3f865bc6c9ce364e6cefc7f094d73383860732afe5`.
- A previously missing unsigned-remainder import was resolved to `/usr/lib/libgcc_s.1.dylib` using the original import ordinal and SDK, with independent image inspection.
- **Diagnostic subset linking gets zero whole-game replacement credit** and does not establish real iOS runtime operation.

## CI and evidence

- PR #3 is **open and draft**, at head `1f25b660eb1`; the [PR page](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3) may continue to change.
- Both latest [build-and-report run 37781126678](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37781126678) and [companion run 37781116326](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37781116326) **passed** for that exact head, with macOS/Ubuntu tests and the historical candidate-progress job.
- The [historical-progress job](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37781126678/job/113324192479) printed `411/9177 verified functions; 4864 matched bytes; 8732 missing; 0 unresolved; 0 compile errors`. A deliberate source change reduced the result to 410/4,856 bytes, and rebuilding restored 411/4,864 with the regression test passing.
- The PR reports **92 tooling tests and `doctor` passing locally**; this status check corroborated hosted CI outcomes, but did not rerun its entire local test suite.
- The latest head has 67 configured groups, with the source and actual SDK/compiler binary dependencies kept outside Git. Repository visibility is currently public.

## Known technical constraints

1. **Compiler fidelity:** original per-unit flags and the exact shipped compiler/backend/assembler/linker are unproven. The reconstructed bundled LLVM ARM backend currently cannot reproduce some original tail-call and Darwin global-address code sequences.
2. **Unresolved codegen patterns:** many nonexact map equality and audio predicate bodies differ in MOV/CMP ordering; optimizing source or swapping modern Clang must not be allowed to claim matches without complete exact-byte evidence.
3. **Partial ABI:** class inheritance, virtual slots, smart-pointer/allocator lifetime behavior, enum values, signedness and object sizes are not fully reconstructed. Some source bodies make provisional layout assumptions.
4. **Data/link gap:** source-backed exact data remains four bytes; all complete replacement-link units remain zero. Structurally linked subsets are useful tooling tests, not a bootable game.
5. **Target port:** a modern 64-bit iOS build will require runtime/API modernization and integration beyond matching the historical ARMv7 app.

## Recommended next actions

1. **Keep PR #3 CI and its description synchronized:** the latest 411-function head CI is now green, even though the PR body still says it needs a run.
2. **Identify the high-value game subsystems and group ownership** across the 268 object groups. Track third-party/library, engine, platform and Pirates-specific source separately.
3. **Use representative complicated functions as stress tests** (real branches, imported calls, data, vtables and control flow) rather than judging throughput from short accessors alone.
4. **Investigate codegen fidelity** (tail calls, Darwin address materialization, MOV/CMP scheduling) using bounded compiler/flag experiments and explicit unverified status.
5. **Continue exact-data and diagnostic-link research** without relaxing the original-layout/full-image coverage gates or claiming a runnable replacement prematurely.

## Status-report history

The previous 91-function and 236-function snapshots remain accessible in [`STATUS.md` Git history](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commits/main/STATUS.md). Canonical changing data lives in the candidate branch's `build/report.json`, CI summaries, [`docs/recovery-loop.md`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/blob/1f25b660eb1b145809fe9ed72ff495d0c4644bbd/docs/recovery-loop.md) and the active [PR #3](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3).

**Assessment:** The project now has a credible, regression-protected, compiler-validated source-recovery workflow that can operate across many original groups. The decisive next stage is demonstrating similar effectiveness on larger, interconnected *Pirates!* gameplay code—not merely growing the count of tiny exact functions.
