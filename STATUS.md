# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026 (Europe/Zurich)  
**Repository:** [dberweger2017/decomp-sid-meier-s-pirates](https://github.com/dberweger2017/decomp-sid-meier-s-pirates)  
**Merged `main` baseline:** [`4bcbb27e4f70`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/4bcbb27e4f7020c57dabc4d708fbf72180325dfa) (previous status report only; code baseline from merged PR #2)  
**Active draft:** [PR #3 — Recover easy ARMv7 functions one at a time](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3), head [`1c4b4a261020`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/1c4b4a261020f65e7261ee3eba93a54c8817a1a5)

> **Bottom line:** The historical matching, SDK, browser and diagnostic-linking tools are established; small-function recovery is accelerating. The active branch has **91 verified functions / 1,332 exact code bytes**, plus a **4-byte matched data allocation**. This is **not** a playable replacement, a full-game link, or a modern arm64 iOS port. Numbers below distinguish merged `main` from unmerged PR #3 work.

## What changed since the preceding status report

| Metric | Previous snapshot (PR #3 at `d559a7a`) | Current PR #3 at `1c4b4a2` | Change |
|---|---:|---:|---:|
| Exact matched functions | 54 | **91** | **+37** |
| Exact matched function bytes | 968 | **1,332** | **+364** |
| Source-backed exact data | 4 bytes | **4 bytes** | No change |
| Full replacement linking | 0 units | **0 units** | No change |
| Main branch code status | 16 / 628 bytes | **16 / 628 bytes** | No code merge yet |

The active branch now contains **75 more exact functions and 704 more exact code bytes than merged `main`**. In addition to earlier GameAudio, PowerVR string/resource and FSound work, the recovery loop added **21 FSound3D methods** (the 75-function / 1,188-byte checkpoint) and **16 FAudioManager accessors/setters** (the 91-function / 1,332-byte checkpoint). Each new exact function is described as having its own commit.

The new matches are mostly **short accessors, setters, release-build no-ops and related glue**, useful for validating the recovery workflow but not yet a demonstration of reconstructing complex gameplay. Do not infer decompilation throughput for the remaining difficult functions from this batch.

## Progress against the original binary

| Measure | Merged `main` | Active PR #3, not merged |
|---|---:|---:|
| Original STABS function records | 9,177 | 9,177 |
| Original object groups | 268 | 268 |
| Original STABS function bytes | 4,080,584 | 4,080,584 |
| Exactly matched functions | **16** | **91** |
| Exactly matched function bytes | **628** | **1,332** |
| Fraction of function records matched | 0.1743% | **0.9916%** |
| Fraction of identified function bytes matched | 0.0154% | **0.0326%** |
| Differing source candidates | 5 | **5** |
| Missing function candidates | 9,156 | **9,081** |
| Original non-code data inventory | 5,056 ranges / 3,827,236 bytes | Same |
| Exact matched non-code data | 0 bytes | **4 bytes** (one `CPVRTString::npos` allocation) |
| Finished replacement-link units | 0 | **0** |
| Runnable replacement / modern iPhone build | No | **No** |

The **percentage of functions matched** and **percentage of original function bytes matched** measure different things. They must not be interpreted as percentage of engineering effort completed, nor as a forecast for reaching a playable game. Data bytes and full-image linking have separate accounting, and isolated diagnostic links earn no replacement credit.

## Milestones and infrastructure

### Merged PR #1 — Historical workbench

- Imported/identified the archived 32-bit ARMv7 iPhone v1.1.2 executable and verified it against recorded hashes. The IPA has **archival**, not authenticated Apple-CDN, provenance.
- Inventoried STABS functions and original object groups while retaining unity compilation structure, symbol boundaries and ARM/Thumb modes.
- Built function selection, Ninja incremental builds, source/header watching, disassembly differences, supported relocation resolution, conservative exact-byte checks, compiler diagnostics and reproducible progress reports.
- Reconstructed an **Apple LLVM-GCC 4.2.1 / LLVM build 2336.9** ARM Mach-O object compiler from pinned sources, with synthetic C/C++ probes validated across Linux and Apple Silicon Docker.

### Merged PR #2 — SDK, historical linker and first candidates

- Added content-pinned **iPhoneOS 5.1 / build 9B176 SDK**; the published mirror is **not authenticated as the original Apple archive**.
- Validated C, C++, Objective-C and Objective-C++ ARM/Thumb object compilation.
- Built a separately pinned historical **ld64 linker** with structural SDK-dependent tests, repeatable binary fingerprints and explicit missing-import rejection. These probes do **not** demonstrate execution on real iOS.
- Created a 5,056-allocation non-code data inventory, a guarded data-comparison mechanism, a strict replacement-link gate and separate fuzzy/exact/data/linking measures.
- Established 16 verified source functions / 628 bytes as the merged baseline. Full original flags, compiler fidelity and linker layout remain unproven.

### Draft PR #3 — One-function-at-a-time source recovery

- Committed exact source matches for GameAudio, CPVRTString, CPVRTResourceFile, FSound, FSound3D and FAudioManager accessors. Its candidate manifest configures **11 original compilation groups**; most are partial.
- Recorded an exact **4-byte `CPVRTString::npos` data allocation** at the observed section and alignment rather than relying on default compiler placement.
- Preserved the nine no-op/constant-return GameAudio entry points as observed in this particular shipping build; this does not recover a hypothetical alternate audio implementation.
- Ran isolated **diagnostic** ARMv7 structural links for GameAudio and increasingly broad audio-accessor subsets (including a five-group subset). They are not replacements for the original executable and do not establish runtime behavior.
- The [recovery loop guide](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/blob/1c4b4a261020f65e7261ee3eba93a54c8817a1a5/docs/recovery-loop.md) documents the discipline: identify one original symbol/boundary, write readable source, compare relocated bytes with historical compilation, protect earlier matches and commit only after exact verification.

## GitHub and validation status

- **Merged PRs:** [#1](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/1) and [#2](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/2).
- **Active:** [#3](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3), open **draft**, 84 commits and 19 changed files at capture.
- The **54-function** [previous snapshot's CI](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37761321211) later finished successfully.
- The **75-function checkpoint** [passed build-and-report CI](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37762635804).
- The **91-function head** has a [running retried build](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37763813683): Ubuntu synthetic tooling **passed**, macOS synthetic test steps had passed but its job was not yet fully complete, and the real historical-candidate progress job was **still running** after compiler validation. Do **not** call this entire head green yet.
- A prior [91-function workflow attempt](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37763808228) failed very early. The job logs were unavailable when checked; **its root cause is not confirmed**. No verified game-code regression should be asserted from that status alone.
- PR #3 reported **85 locally passing tests at an earlier checkpoint**. This snapshot does not claim independently rerunning local tests at the latest head.
- The repository's visibility was returned as **public** in the latest GitHub metadata. Confirm that this change from its earlier private status is intentional; game binaries and SDK files remain configured to stay outside Git.

## Risks, uncertainty, and limitations

1. **Codebase complexity:** recovery is at **0.0326% of inventoried function bytes** on PR #3, with most original functions still missing. The current fast progression is mostly small audio/accessor methods; don't extrapolate its pace to full engine or Pirates gameplay.
2. **Toolchain fidelity:** LLVM-GCC, SDK and ld64 are suitable for verified local cases, but the original per-unit flags, exact shipped compiler/assembler/linker revisions and full binary link layout have not been recovered.
3. **ABI and data uncertainties:** several C++ class declarations contain placeholder or partial layouts. Vtables, complex shared data, imported dyld calls and some interworking/veneer relocations still need investigation; equal bytes in a tiny getter do not validate its entire enclosing class.
4. **Diagnostic vs replacement linking:** successful subset links prove a useful structural capability, but original-image replacement remains blocked, with zero completed units. There is no proof of iOS runtime startup for game code.
5. **Final target is different:** moving from an ARMv7 matching decomp to an arm64 executable involves SDK/runtime/API modernization and integration work beyond matching the original bytes.

## Recommended next steps

1. **Close the PR #3 CI loop** for the current 91-function head, including checking the earlier quick failure. Refresh progress from the native report and merge only when the full head is green and reviewed.
2. **Add at least one meaningfully nontrivial source match**: branches, calls, relocations, imports, shared data or more realistic object layouts. Small getters establish plumbing, not comprehensive engine support.
3. **Classify and prioritize the 268 original groups** by vendor-library, iOS glue, engine, and **Pirates-specific gameplay** ownership. Keep subsystem-specific matched-function and byte metrics so progress is relevant to the goal of modifying sailing, naval combat and coastal bombardment.
4. **Expand data/ABI confidence** using selected constants, globals, vtables and Objective-C metadata with exact section placement and relocation evidence. Continue honest zero credit for diagnostic links.
5. **Maintain reproducible recovery evidence**: per-function source, original address, candidate flags, compiler/SDK fingerprint, exact-byte/relocation result, changed input hashes and CI regression status.

## Provenance and report lifecycle

This is a **point-in-time status report**. Its predecessor is preserved in [Git history](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commits/main/STATUS.md). For changing status, consult the current branch's `build/report.json`, the [PR #3](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3) diff and CI, [docs/progress.md](docs/progress.md) and the branch's [docs/recovery-loop.md](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/blob/1c4b4a261020f65e7261ee3eba93a54c8817a1a5/docs/recovery-loop.md).

**Overall assessment:** The original compiler/reproducible tooling milestone is largely established. The agent is demonstrating a disciplined small-function recovery loop, with one verified data item and repeatable diagnostic linking. The highest-value next proof is that the workflow scales to realistic, interconnected game code.
