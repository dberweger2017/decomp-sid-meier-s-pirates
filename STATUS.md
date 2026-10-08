# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026.  
**Merged website/tooling baseline:** PR #5, `23b86a0593d10041c0040d337c00f28bf851a136`.  
**Active recovery:** [PR #7](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/7), stacked on [PR #4](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/4).  
**Separate partial animation recovery:** [PR #6](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/6).

PR #7 adds **500 exact small functions / 8,000 code bytes** to the 811-function
PR #4 checkpoint. All earlier matches remain verified. This batch is primarily
ABI entry forwarding; the larger callees and complete class definitions remain
separate recovery work. Full-game replacement linking stays at zero.

| Measure | Merged main | PR #4 checkpoint | PR #7 checkpoint |
|---|---:|---:|---:|
| Named functions / original groups | 9,177 / 268 | 9,177 / 268 | 9,177 / 268 |
| Original inventoried function bytes | 4,080,584 | 4,080,584 | 4,080,584 |
| Verified exact functions | 411 | 811 | **1,311** |
| Verified exact code bytes | 4,864 | 6,892 | **14,892** |
| Exact functions by count | 4.4786% | 8.8373% | **14.2857%** |
| Exact original code bytes | 0.1192% | 0.1689% | **0.3649%** |
| Configured original compilation groups | 67 | 115 | **153** |
| Differing candidates | 34 | 40 | 40 |
| Missing function candidates | 8,732 | 8,326 | 7,826 |
| Compile errors / unresolved comparisons | 0 / 0 | 0 / 0 | 0 / 0 |
| Matched data | One four-byte allocation | Same | Same |
| Completed replacement code/data/units | 0 / 0 / 0 | 0 / 0 / 0 | 0 / 0 / 0 |

The 500 new functions comprise 255 complete-destructor wrappers, 233 method
forwarders, six free-function forwarders and six complete-constructor wrappers.
They have a separate verified commit for each of their 92 original groups.
Source expresses ordinary C++ calls with ABI symbol declaration labels, never
assembly instructions or original-byte bodies. Unrecovered callees stay missing;
no placeholder implementation earns progress. Opaque pointers, complete types,
original flags and unencoded result types remain provisional. See
[the batch's source method and limits](docs/small-function-batch.md).

The 9,177-record inventory contains 1,682 records of at most 16 bytes. Of these,
1,430 have simple no-op/identity, trap, constant-return, single-field-access or
direct-call shapes. After this batch, 263 of those simple-shaped records remain
missing. A tiny size does not guarantee an immediate match; ABI, return semantics,
compiler-generated entries and metadata still need investigation. Small-function
counts do not estimate remaining gameplay effort.

The validated historical LLVM-GCC cross-build is reproducible. Equivalence to
Apple's exact shipped backend and original per-unit flags remains unproven.
Exact progress requires supported relocation resolution and complete byte
equality. Assembly similarity never earns verified code or linking credit.
PR #4's larger connected gameplay candidates remain byte-different, with separate
fuzzy scores and bounded ARM execution checks. PR #6's animation candidates are
not included in PR #7's new-function count or totals.

The three-function particle LinkObject source chain structurally links with all
three exports independently inspected. Image SHA-256:
`6b014c2ac464bd1292b4d86cca9748ee234e786d0278f9468e2ff25f8d05b548`.
Existing diagnostic graphs retain explicit earlier source-symbol roots while
new unreachable entries are dead-stripped; missing dependencies are not faked.
The 73-group retained-source diagnostic image SHA-256 is
`1b7b5e3c9081d100cdf13b7cc400679b29d57f7a8d2cd9e1366c3546e96a2f46`.
These subset images grant zero complete replacement or iOS runtime credit.

Local validation: **110 tooling tests pass**, doctor passes, all 500 reviewed
forwarder sources validate, and the particle, connected-gameplay and retained
73-group diagnostic links pass structural inspection. Native regression checks
find exactly 500 new matches and no lost function/data matches. Hosted checks
for the final head are pending; local success is not presented as hosted success.

The public website reflects merged main, not unmerged recovery PRs. Original
IPA/executable inputs, SDKs, toolchain caches and generated artifacts stay out
of Git. No runnable replacement or arm64 port is established.
