# Sid Meier's Pirates! iOS decompilation — status report

**Snapshot:** October 8, 2026.  
**Merged baseline:** PR #3, merge commit `e775bfff81964190939d1679869f89e3de66be86`.  
**Active draft:** [PR #4](https://github.com/dberweger2017/decomp-sid-meier-s-pirates/pull/4).

The active branch adds **200 exact functions / 976 code bytes** to the merged
411-function baseline. It also keeps six larger connected gameplay candidates
visible as byte-different, with separate fuzzy scores and bounded execution
checks. Full-game replacement linking remains at zero; no runnable replacement
or arm64 port is established.

| Measure | Merged main | PR #4 local checkpoint |
|---|---:|---:|
| Inventoried named functions / original groups | 9,177 / 268 | 9,177 / 268 |
| Original inventoried function bytes | 4,080,584 | 4,080,584 |
| Verified exact functions | 411 | **611** |
| Verified exact code bytes | 4,864 | **5,840** |
| Exact functions by count | 4.4786% | **6.6579%** |
| Exact original code bytes | 0.1192% | **0.1431%** |
| Configured original compilation groups | 67 | 102 |
| Differing candidates | 34 | 40 |
| Missing function candidates | 8,732 | 8,526 |
| Compile errors / unresolved comparisons | 0 / 0 | 0 / 0 |
| Matched data | One four-byte allocation | Same |
| Completed replacement code/data/units | 0 / 0 / 0 | 0 / 0 / 0 |

All earlier exact function and data matches remain verified. The new batch
contains 156 observed no-op release hooks and 44 constant-return functions,
committed in their 51 original groups. These tiny functions provide modest code
coverage; their count does not estimate remaining development effort. They
reproduce observed shipped bodies, rather than supplying stubs for absent code.
Partial types, unencoded result types and complete virtual hierarchies remain
unproven. See [the recovery ledger](docs/recovery-loop.md).

The historical LLVM-GCC compiler cross-build is validated, but equivalence to
Apple's exact shipped backend and original per-unit flags remain unproven.
Recovered source uses ordinary C++, with no original instruction payloads or
assembly fallbacks. Exact matches require supported relocation resolution and
byte equality; fuzzy similarity never receives exact-match credit.

The connected gameplay experiment includes the complete two-function world-map
projection utility chain and four battle-grid/property operations. The square
update is 78.0488% similar; layer enable is 95%; both remain byte-different.
The 102 bounded ARM differential scenarios passed before this release-hook
batch. Those finite modeled trials grant no exact or runtime credit. Larger,
interconnected exact gameplay recovery remains an open milestone. See
[the experiment and its limits](docs/gameplay-stress.md).

The latest local 51-group diagnostic subset structurally links 369 exact
functions / 2,532 code bytes and 21 differing candidates. Its independently
inspected image SHA-256 is
`196801ea118306dd6078bf051cd588fc3a38698b62d8e4b30423efbbbdecba95`.
It verifies subset structure and SDK import bindings, with zero complete
replacement credit and unverified iOS runtime behavior. Reproduce with
`python tools/recovery_link.py config/diagnostic-links/recovery-release-hooks.json`.

Local validation passes **104 tooling tests**, the 200-function cohort ledger,
and the source-only subset link. Hosted CI for this checkpoint is pending.
The prior PR #4 historical CI failure came from a nested workspace replacing the
validated linker profile with the generic template; the fix preserves the
profile and its validation proof, with a synthetic regression test.

Original IPA/executable inputs, SDKs, toolchain caches and generated artifacts
remain outside Git. Decompilation, linking and runtime progress remain separate.
