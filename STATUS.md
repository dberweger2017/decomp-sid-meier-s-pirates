# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot date:** 2026-10-08 (Europe/Zurich)  
**Repository:** [dberweger2017/decomp-sid-meier-s-pirates](https://github.com/dberweger2017/decomp-sid-meier-s-pirates)  
**Merged baseline:** `main` at [`920d7daea53`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/920d7daea53f98988c0c8dc23fb345d72a0af1d9)  
**Active work:** draft [PR #3 — Recover easy ARMv7 functions one at a time](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3), snapshot at [`d559a7aa66a`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/d559a7aa66a1dcfe0e1033dd61790cd2e18ddf0d)

## Executive summary

**The historical source-to-binary matching workbench is functional, and small-scale source recovery is underway.** PR #1 and PR #2 are merged. The project has reconstructed and validated a historical LLVM-GCC/ARM Mach-O object-compilation workflow, pinned an iOS 5.1 SDK and an ld64 linker, verified initial source matches, and added conservative data and replacement-link progress tracking. PR #3 expands recovery function by function while retaining regression checks. **There is no playable or complete replacement executable and no modern arm64/iPhone port.**

This report distinguishes **merged `main`** from **unmerged draft-PR work**. The PR and its CI results may advance after this snapshot.

## Quantitative progress

| Measure | Merged `main` | PR #3 snapshot (unmerged) |
|---|---:|---:|
| Original STABS function records | 9,177 | 9,177 |
| Original object groups | 268 | 268 |
| Identified original function bytes | 4,080,584 | 4,080,584 |
| Exactly matched functions | **16** | **54** |
| Exactly matched function bytes | **628** | **968** |
| Function-record coverage | 0.1743% | 0.5884% |
| Exact code-byte coverage | 0.0154% | 0.0237% |
| Inventoried non-code allocations | 5,056 / 3,827,236 bytes | Same original inventory |
| Exactly matched non-code data | **0 bytes** | **4 bytes** reported in latest branch commit |
| Completed replacement-link units | **0** | **0** |
| Playable replacement / arm64 iOS version | **No** | **No** |

The two matched metrics use different denominators: complete functions versus bytes in all inventoried STABS function ranges. **They are not estimates of work completed or time remaining.** The data figure on the PR branch is from the `CPVRTString::npos` source/data commit at `d559a7a`; the latest head's game-progress CI was still running at capture time.

## Milestones reached

### PR #1 — Original binary, compiler and workbench (merged)

- Identified the original **32-bit ARMv7 iPhone v1.1.2** binary from an archival IPA, checked against recorded SHA-256 values. The source is **not authenticated as an Apple CDN original**; see [identity record](config/identity.json).
- Recovered **9,177 STABS `N_FUN` records** and **268 `N_OSO` object groups**, with deterministic inventory locks. Original unity-compilation groups remain intact.
- Built the native/local-browser matching workbench: incremental Ninja compilation, ARM/Thumb instruction comparison, supported Mach-O relocation resolution, exact-byte verification, source/header watchers, diagnostics, native reports and an objdiff progress-schema adapter.
- Reconstructed **Apple LLVM-GCC 4.2.1 / LLVM build 2336.9** from pinned open sources, using an ARM Mach-O assembler, with repeatable C/C++ ARM/Thumb object probes on native Linux and Apple Silicon through Docker.

### PR #2 — SDK, more language support, linking evidence (merged)

- Pinned the **iPhoneOS 5.1 / build 9B176 SDK**, with content fingerprinting; its current source is a **third-party mirror, not Apple-authenticated**.
- Validated **C, C++, Objective-C and Objective-C++** ARM/Thumb object-compilation probes.
- Added a separately pinned historical **ld64 linker** and reproducible structural SDK link probes (including Objective-C metadata, imports and startup). These demonstrate structural linking, **not runtime execution or equivalence to the original shipped linker**.
- Added a 5,056-range non-code allocation inventory, conservative data matching and diagnostic versus replacement-link separation.
- Merged **16 readable, source-backed PowerVR matches / 628 bytes**, with five additional candidates still differing. Historical compiler flags and exact shipped compiler equivalence remain unproven.
- Built CI safeguards against lost exact matches, stale input/header/SDK/compiler/linker state, lost inventory and invalid linking credit.

### PR #3 — Recovery loop (active draft)

- Reported checkpoint: **54 exactly matched functions / 968 bytes**, **+38 functions / +340 bytes** over merged `main`, committed as individually verified changes.
- Recovery focuses on release-build **GameAudio** no-op/return-zero entries, **PowerVR string/resource accessors**, and **Fireplace/FSound** small field-accessor functions. These are early, generally small functions; they do not constitute broad gameplay-system recovery.
- Added a source-backed **4-byte `CPVRTString::npos` data match** at the observed section/alignment. This is the first exact data allocation claimed on the recovery branch.
- Two **isolated diagnostic links** are reported: GameAudio alone, and a four-group accessor/FireSound subset. They verify structural link feasibility and earn **no replacement-link completion**.
- The recovery-loop guide emphasizes inspecting each original function, recording ABI uncertainties, accepting only relocation-resolved byte equality, preserving prior matches and committing each verified function separately. See [the PR's recovery guide](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/blob/d559a7aa66a1dcfe0e1033dd61790cd2e18ddf0d/docs/recovery-loop.md).

## Validation / CI snapshot

- [Last merged `main` build](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37756001543): **passed** on the merged PR #2 state.
- [PR #3 previous commit build](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37760557098) at `a3d4358e3b0`: **passed**.
- [PR #3 latest-head build](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37761321211) at `d559a7aa66a`: synthetic tooling checks passed, but **game-progress job was still in progress at snapshot time**. Do not mark the whole head green until it finishes.
- PR #3's description reports **85 local tooling tests passing**, no verified function regressions and no compile errors. These are reported results; the present status document does not independently rerun the suite.

## Risks and limitations

1. **Toolchain fidelity:** the 2012 app's metadata identifies LLVM-GCC, Xcode 4.3.2 and SDK 5.1, but the exact shipped compiler/linker build, per-translation-unit flags and ABI details are not fully proven.
2. **Coverage and complexity:** source recovery is still under **0.03% of inventoried function bytes**, and the newest matches are disproportionately short accessors/no-ops. Most complex engine and Pirates-specific behavior has not been reconstructed.
3. **Incomplete relocations/data/layout:** some imported dyld targets, veneers/interworking and original linker transformations remain unsupported or unresolved. One matched four-byte constant does not imply overall data recovery.
4. **Portability versus matching:** generating byte-matching ARMv7 objects is not the same as building and running a full replacement, nor does it solve the eventual modern **arm64 iOS** port and runtime/API migration.
5. **Provenance:** the archived IPA and mirrored SDK have pinned hashes and reproducible local identities, but their provenance is not independently authenticated against original Apple distributions.

## Next recommended milestones

1. **Finish and review PR #3's head CI.** Maintain exact-function, exact-data and link-regression gates before merging; update counts from generated reports rather than commit descriptions alone.
2. **Move beyond easy accessors.** Match representative larger functions with branches, call relocations, imported symbols and shared data, to expose tool limitations early.
3. **Prioritize subsystem mapping.** Classify the 268 original groups into third-party libraries, platform glue, engine and game-specific code. Track each separately, especially the sailing, ship combat, economy and coastal-bombardment systems.
4. **Grow data and link evidence without false credit.** Verify selected vtables/constants/Objective-C metadata and isolated link inputs, while keeping replacement completion at zero until whole-image coverage and equality gates pass.
5. **Keep the recovery loop agent-friendly.** Preserve reproducible per-function source, flags, assumptions, diffs, failure reasons and regression tests; periodically assess matching rate on nontrivial code rather than only increasing easy-function counts.

## Source of truth

For *current*, not snapshotted progress, use `build/report.json`, `build/objdiff-report.json`, the active branch CI summary, [docs/progress.md](docs/progress.md) and [PR #3](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3). Original binary and SDK content are deliberately excluded from Git.

**Bottom line:** the key tooling feasibility milestones have been demonstrated. The work has entered genuine source and data recovery, but a playable modern iPhone build remains a separate, much larger milestone.
