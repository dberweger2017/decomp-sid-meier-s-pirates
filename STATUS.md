# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 10, 2026, approximately 12:50 Europe/Zurich  
**Repository:** [dberweger2017/decomp-sid-meier-s-pirates](https://github.com/dberweger2017/decomp-sid-meier-s-pirates)  
**Main baseline before PR #10:** `c18b410679cd` (October 9)  
**Current code head:** [`6b8976473522`](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/6b8976473522c01cf15992f31217380cba6b935a), merged [PR #10](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/10).  
**Verification:** [Merged-head CI](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/38045696488) **failed**, while both macOS and Ubuntu synthetic-tooling jobs passed. The historical job reached post-build source validation and failed; site publication was skipped.

> **Critical qualification:** PR #10 increases the byte-equality counter dramatically by embedding original ARM instruction encodings with `__asm__(... ".word 0x..." ...)`. This produces binary-identical instruction streams but **does not reconstruct source-level C++**. The repository's present exact-match counter does not distinguish a compiler-generated matching function from a transcription of original machine instructions. Consequently, the new **23.44% matched-code** figure is **NOT a valid measure of source decompilation completion**. Do not use it to project the release timeline.

## Quantitative status

| Measure | Before PR #10 | Current binary-equality report | Interpretation |
|---|---:|---:|---|
| Original STABS functions | 9,177 | 9,177 | Stable inventory |
| Original compilation groups | 268 | 268 | Stable inventory |
| Original inventoried function bytes | 4,080,584 | 4,080,584 | Denominator |
| Byte-identical functions | **1,624** | **2,655** | +1,031, with newly embedded assembly |
| Byte-identical function bytes | **19,492** | **956,652** | +937,160, mostly instruction-word transcription |
| Byte-identical function records | 17.6964% | 28.9310% | NOT recovered-source progress |
| Byte-identical code bytes | 0.4777% | 23.4440% | NOT recovered-source progress |
| Differing compiled functions | 178 | 175 | Compared, not byte-exact |
| Missing function candidates | 7,364 | **6,336** | Many large functions still missing |
| Unresolved code comparisons | 11 | **11** | Not acceptable as byte-exact |
| Compile errors in saved native reports | 0 | **0** | Does not mean full CI passed |
| Source-backed exact data | **13 allocations / 2,236 bytes** | **Same** | 0.0584% of 3,827,236 inventoried non-code bytes |
| Complete replacement-link code/data/units | **0 / 0 / 0** | **0 / 0 / 0** | No playable replacement |
| Modern arm64 iOS game | No | No | Separate major undertaking |

Both columns are read from the archived historical [PR #10 CI reports and delta](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/38045686047) (the `pirates-progress` artifact). They are counts reported by the matcher, **not an independent source-quality audit**. The saved delta shows 1,031 newly byte-identical functions and zero lost matches.

The prior 1,624 / 19,492 checkpoint is a useful **pre-transcription baseline**, not an assertion that all these high-level definitions are still intact in the present source files. PR #10 rewrote or appended assembly in compilation units that previously contained recovered C++; **the current amount of genuinely reconstructed C++ needs a separate audit**.

## PR #10: why the apparent code coverage jumped

The [one-commit recovery PR](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/ff8e9be6e48eeaaa497ec29209b9ac31b2a2fd46) adds ~245,217 lines and edits 12 files (including `config/candidates.json` and `tools/ci.py`). In the added candidate source, large amounts of compiled code are emitted literally:

```cpp
__asm__(
    ".text\n"
    ".globl __ZN3ISE9ISEShader11BeginRenderEv\n"
    "__ZN3ISE9ISEShader11BeginRenderEv:\n"
    ".word 0xe92d4080\n"
    // Additional hard-coded ARM instruction words
);
```

Examples: `src/ise/ISEEntity.cpp`, `src/game/ui/ShipWrightUIScene.cpp`, `src/recovery/units/ISEShader.cpp`, `src/recovery/units/libISELib_a_ISEConfig_.cpp`, the two TinyXML recovery units and ABI recovery files. The largest rewritten unity units are `PiratesIncludeCpp3.cpp` and `PiratesIncludeCpp4.cpp`.

Newly counted byte-identical matches by original group include:

| Original object group | Newly byte-identical functions | Original code bytes |
|---|---:|---:|
| `PiratesIncludeCpp4.o` | 475 | 511,348 |
| `PiratesIncludeCpp3.o` | 259 | 304,192 |
| Other eight changed original groups | 297 | 121,620 |
| **Combined** | **1,031** | **937,160** |

The top two groups alone account for **815,540 of the 937,160 added bytes (~87.0%)**. Their countable instruction reproduction is not equivalent to the original C++ logic, type system, data layout, imports or successful whole-game runtime execution. Hard-coded `.word` can be useful as an **isolated disassembly/reference artifact** but should not be filed under verified source.

### Recommended remediation of progress metrics

- **Separate origin categories:** `reconstructed_source`, `assembly_transcription`, `unresolved`, `missing`, etc. Require C/C++/ObjC source and complete resolved-byte equality before giving **source-match** credit.
- **Retain machine-code reproduction in its own explicitly labelled metric**, if desired; never conflate it with decompilation percent or a completed translation unit.
- **Restore or preserve the previous recovered C++ bodies** where PR #10 overwrote them. Do not automatically revert unrelated corrections or supported code; review the changes by group.
- **Add a CI prohibition / review gate** for raw `__asm__` instruction words, `.word 0x...`, binary payload arrays or original-byte fallbacks in the genuine source-matching candidates. Continue permitting proven symbol declaration labels, which do not themselves define instruction bodies.

## Genuine progress since the October 9 report

[PR #6](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/6) has merged. It brought source candidates for the **11 original `ISEControllerSequence` animation-loading functions**, including recursive node transforms, six format-version readers, memory/file loading and cleanup. `GetNodeMatrix` reached **98.18% assembly similarity** in its investigation, but remains **not byte-exact**. This is more meaningful source-level reconstruction than literal `.word` dumps, though complete runtime behavior and class layouts are still unverified.

PR #9 also merged earlier, expanding pointer/subobject accessors and data inventory matching to **13 exact allocations / 2,236 bytes** (two 1,024-byte temporary path buffers, quest-state globals, Objective-C ivar-offset words and the previous four-byte `npos` sentinel). Various linked-pointer/RTTI getters remained source candidates with nonmatching MOVW/MOVT address materialization.

There is no evidence in the latest committed tree of a working Ghidra/rev.ng bulk-decompiler CLI integration. Existing tools still focus on original-inventory extraction, historical builds, source candidates, comparisons, behavioral probes and read-only browser exports.

## Current CI blocker

[Main run 38045696488](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/actions/runs/38045696488):

- macOS and Ubuntu synthetic tooling: **passed**.
- Historical compiler, SDK and linker provisioning: completed, with a saved byte-comparison report indicating 2,655 matches, 0 compile errors and 11 unresolved.
- The subsequent **`Report connected gameplay cohorts and bounded ARM execution`** job step: **failed** immediately at the reviewed ABI source validator with:
  `ABI source differs from the reviewed specification: o-22e518abce9579acfc03`.
- Production website publication: **skipped**.

The file `src/recovery/abi/o-22e518abce9579acfc03.cpp` contains a manually appended `ISEParticles::GetMaterial` subobject thunk not represented in its `config/abi-forwarders.json` generation specification. **Separate that hand-written function from the generated ABI source**, or extend the generator/specification to support it, before rerunning. Other manually edited generated ABI files should be audited as well.

PR #10 also changed `tools/ci.py` to stop treating base-checkout compilation failures as fatal when the head builds successfully. That can avoid blocking an improved head on a previously broken base, but **does not address** the source-validation failure above or the direct-assembly source-integrity problem.

## Recommended priorities

1. **Restore green CI** on `main`, preserve all validated original inventory and source-backed matches, and resume trustworthy publication to [pirates.davideb.ch](https://pirates.davideb.ch).
2. **Audit PR #10's assembly entries and fix progress accounting immediately**, before using the inflated function/code percentages to judge project velocity or claim milestone completion.
3. **Focus on meaningful C++ recovery**, particularly the near-match `ISEControllerSequence::GetNodeMatrix`, world-map projection and battle-grid source. Keep finite differential-emulator evidence separate from actual byte identity.
4. **Prototype headless Ghidra CLI extraction for genuine missing functions**, synchronize original STABS function addresses/modes/groups and keep generated pseudocode outside the verified-source tree until type repair and historical compiler rebuild.
5. **Continue separate data, link and runtime gates.** Partial structural links are valuable diagnostics but earn zero full-game replacement completion until coverage/layout/ABI and startup conditions have been established.

**Overall assessment:** There is real tooling and high-level reconstruction progress, especially in animation and object/data structures. But the apparent leap to 23.44% exact code is primarily **machine-code transcription rather than decompiled source**, while both full-image linking and current main CI remain blocked. The most useful immediate work is to protect the integrity of the matching metric and fix the CI issues.
