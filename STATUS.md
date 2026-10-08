# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026.  
**Merged website/tooling baseline:** PR #5, `23b86a0593d10041c0040d337c00f28bf851a136`.  
**Active recovery:** [PR #8](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/8), stacked on [PR #7](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/7) and [PR #4](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/4).
**Separate partial animation recovery:** [PR #6](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/6).

PR #8 verifies **all 263 remaining records in the tiny-function shortlist /
2,632 additional code bytes**. All 1,311 earlier matches and the four-byte data
match remain intact. Each of the 131 affected original groups has a recovery
commit. One destructor entry was independently recovered on PR #6; this branch
now includes its ABI wrapper without the separate unfinished animation work.
The batch recovers small entry bodies, not complete gameplay algorithms or
class/runtime metadata. Full-game replacement linking stays at zero.

| Measure | Merged main | PR #4 | PR #7 | PR #8 |
|---|---:|---:|---:|---:|
| Named functions / original groups | 9,177 / 268 | Same | Same | Same |
| Original inventoried function bytes | 4,080,584 | Same | Same | Same |
| Verified exact functions | 411 | 811 | 1,311 | **1,574** |
| Verified exact code bytes | 4,864 | 6,892 | 14,892 | **17,524** |
| Exact functions by count | 4.4786% | 8.8373% | 14.2857% | **17.1516%** |
| Exact original code bytes | 0.1192% | 0.1689% | 0.3649% | **0.4294%** |
| Configured original compilation groups | 67 | 115 | 153 | **198** |
| Differing candidates | 34 | 40 | 40 | 40 |
| Missing function candidates | 8,732 | 8,326 | 7,826 | **7,563** |
| Compile errors / unresolved comparisons | 0 / 0 | Same | Same | Same |
| Matched data | One four-byte allocation | Same | Same | Same |
| Completed replacement code/data/units | 0 / 0 / 0 | Same | Same | Same |

PR #8 contains 58 complete-constructor wrappers, 50 SDK-prototyped import
wrappers, 92 destruction callbacks, 13 registration forwarders, 17 empty bodies,
17 constant results, ten Objective-C leaf methods, and one each of a complete
destructor wrapper, destructor cleanup, by-value visitor wrapper, identity body,
empty initializer and adjusted field getter. Private callbacks preserve local
linkage through unrooted source-emission references; these scaffolding pointers
receive no original data or lifetime-registration credit. Objective-C argument
and result types come from original runtime method encodings. See
[the source method and limits](docs/remaining-tiny-functions.md).

The 500 new functions comprise 255 complete-destructor wrappers, 233 method
forwarders, six free-function forwarders and six complete-constructor wrappers.
They have a separate verified commit for each of their 92 original groups.
Source expresses ordinary C++ calls with ABI symbol declaration labels, never
assembly instructions or original-byte bodies. Unrecovered callees stay missing;
no placeholder implementation earns progress. Opaque pointers, complete types,
original flags and unencoded result types remain provisional. See
[the batch's source method and limits](docs/small-function-batch.md).

The 9,177-record inventory contains 1,682 records of at most 16 bytes. The
entire **1,430-record simple-shaped catalogue is now byte-verified**: 628 direct
call wrappers, 373 empty/identity entries, 167 constant returns, 149 single-field
accessors and 113 shipped traps. Across all records of at most 16 bytes, 1,513
are exact, 26 have differing candidates and 143 remain missing. Those remaining
records fall outside the simple-shaped catalogue; their size does not establish
easy ABI, compiler or source recovery. Small-function counts do not estimate
remaining gameplay effort.

The validated historical LLVM-GCC cross-build is reproducible. Equivalence to
Apple's exact shipped backend and original per-unit flags remains unproven.
Exact progress requires supported relocation resolution and complete byte
equality. Assembly similarity never earns verified code or linking credit.
PR #4's larger connected gameplay candidates remain byte-different, with separate
fuzzy scores and bounded ARM execution checks. PR #6's larger animation candidates are
not included in PR #8's totals; its already-recovered destructor wrapper is
explicitly accounted for in this batch.

The three-function particle LinkObject source chain structurally links with all
three exports independently inspected. Image SHA-256:
`6b014c2ac464bd1292b4d86cca9748ee234e786d0278f9468e2ff25f8d05b548`.
Existing diagnostic graphs retain explicit earlier source-symbol roots while
new unreachable entries are dead-stripped; missing dependencies are not faked.
The 73-group retained-source diagnostic image SHA-256 is
`1b7b5e3c9081d100cdf13b7cc400679b29d57f7a8d2cd9e1366c3546e96a2f46`.
These subset images grant zero complete replacement or iOS runtime credit.

Local validation: **117 tooling tests pass**, doctor passes, all 574 reviewed
forwarders and 128 reviewed ABI leaf sources validate, and the fixed 263-record
cohort passes `--require-exact`. Native regression finds exactly 263 new matches
relative to PR #7 with no lost function/data matches. Particle, connected-gameplay
and retained 73-group diagnostic images all preserve their previous SHA-256
identities despite the added unreferenced source entries. Their linked live
source graphs have not expanded merely because more bodies were compiled.
PR #8 CI now requires every selected entry to remain exact while exporting
reports on failure. Hosted final-head checks are pending; PR #7's final Linux,
macOS, historical compilation and compiler cross-build checks passed.

The public website reflects merged main, not unmerged recovery PRs. Original
IPA/executable inputs, SDKs, toolchain caches and generated artifacts stay out
of Git. No runnable replacement or arm64 port is established.
