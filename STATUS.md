# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026, approximately 13:52 CEST (Europe/Zurich)  
**Repository:** [dberweger2017/decomp-sid-meier-s-pirates](https://github.com/dberweger2017/decomp-sid-meier-s-pirates)  
**Merged main baseline:** [`a13f9bd58591`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/a13f9bd58591969c6d082935edc0761f25e05225) (docs-only update above the PR #2 code merge)  
**Active draft branch:** `feat/easy-function-recovery`, [`5d8bea221452`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/5d8bea221452490c6e4f724b637d897fffd9a80e), [PR #3](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/3)

> **Summary:** The validated ARMv7 matching toolchain and workbench now support a repeatable multi-function recovery loop. The draft branch has **236 byte-for-byte verified functions / 2,900 code bytes**, including a **100-function batch** across five original unity compilation groups. The **latest head CI is green**. The original game executable has **not** been recreated, and there is no playable iOS or arm64 port.

This is a time-stamped snapshot. Do not confuse the latest **unmerged** PR #3 progress with the **merged** source on `main`. Code and data completion remain separate from full-image linking.

## Progress at a glance

| Measure | Merged main | Draft PR #3 |
|---|---:|---:|
| Original STABS function records | 9,177 | 9,177 |
| Original object groups | 268 | 268 |
| Inventoried function bytes | 4,080,584 | 4,080,584 |
| Exact verified functions | **16** | **236** |
| Exact verified code bytes | **628** | **2,900** |
| Exact function coverage | **0.1743%** | **2.5716%** |
| Exact STABS code-byte coverage | **0.0154%** | **0.0711%** |
| Differing source candidates | 5 | **7** |
| Missing original function candidates | 9,156 | **8,934** |
| Non-code inventory | 5,056 allocations / 3,827,236 bytes | Same original inventory |
| Source-backed exact data | 0 bytes | **4 bytes**, one `CPVRTString::npos` allocation |
| Completed replacement-link units | **0** | **0** |
| Playable modern iPhone executable | No | No |

Against the [previous STATUS.md snapshot](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commits/main/STATUS.md) at 91 functions / 1,332 bytes, PR #3 gained **145 exact functions and 1,568 exact code bytes**. Against merged main, PR #3 has **220 additional functions and 2,272 additional matched bytes**.

**Interpretation:** Function record coverage is not executable byte coverage. The newest matches skew heavily toward tiny getters, no-ops and UI/accessor functions; these percentages **are not a percentage of remaining engineering work**. The four data bytes are not included in the code-byte total.

## Most important new milestone: a whole batch of 100 matches

The recovery agent changed from editing functions one by one to **investigating approximately 100 related functions together**, preserving the original unity compilation boundaries. This reduces repeated compile/configuration overhead and creates a better test of editor watchers, header dependencies, compiler flags and existing-match regression gates.

The first 100-function batch **exactly matched all 100 candidate definitions (660 code bytes)**, organized across five original unity groups:

| Original group | New verified functions |
|---|---:|
| `FireIncludeCpp.o` | 23 |
| `FireIncludeCpp2.o` | 10 |
| `PiratesIncludeCpp2.o` | 28 |
| `PiratesIncludeCpp4.o` | 38 |
| `PiratesIncludeCpp3.o` | 1 |
| **Total batch** | **100** |

The batch includes initial *Pirates*-specific UI/world accessors, engine accessors, indexed animation/child access, state resets, observed constant-return methods and release-build callbacks. **It does not mean these unity units or gameplay systems are fully reconstructed.** Many source-level return types, class layouts, virtual slots, enums and hierarchy relationships remain hypotheses. The partial types should not be instantiated as complete original objects.

An included `UicButton` header edit was tested: it rebuilt the affected `PiratesIncludeCpp4` unity group while preserving all 236 existing verified function matches and the data match. The local browser API preserved selected-function state; the separate T3 visual preview automation timed out, so no fresh screenshot-based UI validation is claimed.

## Source areas matched so far

- **PowerVR graphics/platform:** shell callbacks, matrix/quaternion/vector operations, string/resource accessors and one constant data allocation.
- **Audio subsystem / Fireplace:** GameAudio shipped-release no-op functions; FSound and FSound3D properties; FAudioManager controls; FAudioSystem; FSharedSoundData; FPhono call wrappers; FKnob and FSoundScape small methods.
- **Early engine and UI/world accessors:** the five Fire/Pirates original unity objects and their included partial sources under `src/fireplace/engine`, `src/fireplace/ui`, `src/game/ui`, `src/game/world` and `src/gamebryo`.
- **Miscellaneous:** a simple heap diagnostic function.

This is still **mostly small-function reconstruction** rather than full sailing, economy, combat, dance, sword fighting or coastal-bombardment logic. The distinction matters for the user's eventual goal of modifying and porting the iOS game.

## Tooling and linker readiness

### Established in merged PR #1

- Original 32-bit ARMv7 Mach-O inventory from the archived 1.1.2 IPA, with STABS boundaries and 268 preserved object groups.
- Historical **Apple LLVM-GCC 4.2.1 / LLVM 2336.9** C/C++ ARM/Thumb Mach-O object compilation with a legacy assembler; provenance and compiler images pinned for reproducibility on Linux and Apple Silicon through Docker.
- Incremental Ninja compilation, local matching browser, source/header watching, assembly comparisons, supported relocation resolution and conservative byte-identity checks.

### Established in merged PR #2

- Content-pinned **iPhoneOS 5.1 SDK, build 9B176**, and four-language (C, C++, Objective-C, Objective-C++) ARM/Thumb object-compilation probes.
- Independently pinned historical **ld64** linker with structural SDK startup/import/Objective-C tests. This is not a game runtime test.
- Separate exact-code, fuzzy-code, non-code-data and full-link accounting; original data layout inventory; regression gates and isolated diagnostic linking.

### Expanded in draft PR #3

- Recovery by related multi-function batches and original unity groups.
- Seven non-exact candidates kept visible rather than counted as matches.
- **16-group diagnostic link** containing **200 recovered functions / 1,776 code bytes**, plus the verified four-byte sentinel. Its recorded SHA-256 is `3bd6fa5feba15291ec494f7dc857df69d50f40209c430dcbc36ce0686397069b`.
- Diagnostic subset images pass independent structural inspection but receive **zero replacement-link credit** and have **no demonstrated iOS runtime behavior**.
- A live editor/API rebuild test confirms dependency invalidation within a shared unity object without losing existing matches.

## Validation and current GitHub status

- **PR #1 and PR #2:** merged to `main`.
- **PR #3:** open **draft**, **143 commits** and **143 changed files** at head `5d8bea221452`; not merged.
- [Latest head CI run `37771932766`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37771932766) and [companion run `37771927844`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37771927844): **both passed**; macOS and Ubuntu synthetic tooling and real historical-candidate progress were green.
- The head historical-candidate [job log](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/37771932766/job/113293371030) independently reports **236 / 9,177 functions, 2,900 matched bytes, 8,934 missing, 0 unresolved and 0 compile errors**. A deliberate source regression reduced matches to 235 / 2,892 bytes, and restoring/rebuilding the candidate returned the result to 236 / 2,900, confirming that the regression gate is active.
- The PR previously reported **85 local tooling tests** at an earlier checkpoint. The report does not imply all tests have been manually rerun outside CI during this review.
- The repository currently appears **public**. Original game binaries, SDK inputs, generated game images and toolchain caches remain excluded from source control per the repository configuration.

## Main technical obstacles

1. **Original compiler/backend equivalence is unproven.** The current bundled ARM LLVM backend disables tail-call lowering; certain original tail branches compile into ordinary calls and stack frames instead.
2. **Darwin global-address code generation differs.** Some original RTTI getters use MOVW/MOVT while the reconstructed backend uses literal pools. Those candidates are correctly deferred rather than granted exact-match credit.
3. **Incomplete structure/relocation knowledge.** The current partial ABI declarations cannot prove class sizes, virtual layout, return types or initialization paths. Imports, veneers and other linker-generated transformations remain a separate challenge.
4. **Real complexity remains.** Exact matching over 2.57% of function records corresponds to only **0.0711% of inventoried function bytes**; most unimplemented code is larger and more interconnected than the latest recovered accessors.
5. **No full-game image / arm64 runtime.** Structural diagnostic links and matching the ARMv7 executable cannot themselves produce a modern runnable iPhone game.
6. **Source provenance.** The archived IPA and SDK mirror are SHA-256/content pinned, but neither is established as identical to an authenticated Apple distribution.

## Next recommendations

1. **Advance from easy-accessor batches to representative, nontrivial game functions.** Include original calls, relocation targets, shared data and actual control flow. Track their byte match and time-to-match separately from trivial getters.
2. **Classify the 268 original object groups** into vendor code, platform glue, Fireplace/Gamebryo engine and Pirates gameplay. Report progress by subsystem so high counts of small vendor functions don't mask the gameplay milestone.
3. **Investigate compiler differences systematically:** identify which original compilation groups require another LLVM backend/flags, particularly tail branches and Darwin RTTI address lowering, instead of altering verified source to compensate improperly.
4. **Grow exact data and structure evidence:** globals, vtables, Objective-C metadata, class offsets and layouts, while retaining independent data/link progress metrics.
5. **Keep PR #3 CI and source documentation current.** Its description still says the 236-function head needs CI, whereas the checked run has now passed; update that when maintaining PR documentation. Preserve historical status snapshots in Git.

## Reporting notes

The canonical progress outputs are `build/report.json`, `build/objdiff-report.json`, the active PR CI summary and the current [recovery-loop documentation](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/blob/5d8bea221452490c6e4f724b637d897fffd9a80e/docs/recovery-loop.md). This status document is a **historical snapshot**, not a live counter.

**Overall assessment:** The infrastructure has transitioned from experimental feasibility to an effective, reproducible multi-function source-recovery workflow. The next decisive demonstration is matching realistic, interconnected Pirates-specific game logic—not simply accumulating more short exact functions.
