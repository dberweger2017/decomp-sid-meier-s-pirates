# Additional small-function recovery

Work branch: `feat/500-small-functions`, stacked on the 811-function checkpoint
in PR #4. The requested target is **500 additional functions**; the earlier
400-function batch and the separately committed ControllerSequence work in PR
#6 are excluded from the new-function count.

Inspect named original bodies, recover ordinary C++ in their original object
and unity groups, compile with the pinned historical LLVM-GCC container, and
verify supported relocation-resolved bytes. Partial source and fuzzy similarity
remain separate from verified functions. Original code/assembly is investigation
input only; no original-byte or assembly implementation fallbacks are counted.

Commit verified groups frequently. Preserve all earlier function/data matches,
run tooling and candidate CI, and periodically inspect source-only diagnostic
links. Subset links do not earn full-game replacement completion.

Complete class layouts, original flags and unencoded return types remain
hypotheses. A small shipped no-op or trap is recoverable only when those are the
actual original instructions; it is not a stand-in for larger missing behavior.

## Verified checkpoint

All **500 additional functions / 8,000 code bytes** verify against original
relocation-resolved bytes, reaching **1,311 exact functions / 14,892 bytes**.
All 811 earlier matches and the four-byte data allocation are preserved. There
are 40 differing candidates, 7,826 missing functions, zero unresolved comparisons
and zero compile errors. Inventory remains 9,177 records in 268 original groups.
The batch spans 92 original groups; 153 groups now have configured candidates.

| Recovered entry kind | Exact functions | Code bytes |
|---|---:|---:|
| Complete destructor → base destructor | 255 | 4,080 |
| Method forwarding | 233 | 3,728 |
| Free-function forwarding | 6 | 96 |
| Complete constructor → base constructor | 6 | 96 |

These are small entry routines, not 500 completed gameplay implementations.
Destruction/allocation/streaming operations in their larger callees remain
separate inventory records and keep their actual missing/nonexact status.
No empty callee bodies are supplied to make a wrapper compile or link.

The source in `src/recovery/abi/` expresses ordinary C++ function calls. Explicit
GNU asm **declaration labels** bind those calls and exported entry points to the
original named ABI symbols; they contain no assembly instructions or byte
payloads. This permits separate recovery of Itanium complete/base constructor
and destructor entry points while their class definitions are incomplete.
Opaque pointer arguments describe ARM register passing only; original references,
pointee types, static/member ownership and return types absent from mangled names
remain provisional. Full class layouts and safe object construction are not
established by these adapters. Replace adapters with normal recovered class
members as each complete class becomes available, retaining the verified IDs.

The reviewed specification is [config/abi-forwarders.json](../config/abi-forwarders.json).
It contains symbol identities, source descriptions and register-level type
hypotheses, with no original code payloads or address overrides. The validator
checks named original call targets, sized/unambiguous ARM boundaries, supported
argument representations, complete/base lifetime relationships and unchanged
forwarding bodies. Unsupported stack/by-value/adjustment cases are excluded.
Ninja still performs the independent, authoritative byte/relocation comparison.

```sh
python tools/abi_forwarders.py config/abi-forwarders.json --check-source
python tools/cohort.py report config/cohorts/abi-forwarders.json --output build/abi-cohort.json
python tools/recovery_link.py config/diagnostic-links/abi-particle-chain.json
```

Native function reports carry `recovery_kind`; the cohort's 100% similarity does
not replace byte verification. The PR contains a separate verified commit for
each original group. The earlier 400-function PR and ControllerSequence PR are
excluded from this batch's 500-function count.

The three-function diagnostic particle chain links `ISEParticlesData::LinkObject`
→ `ISEParticleGeometryData::LinkObject` → the original shipped empty
`ISEParticleObject::LinkObject`. Independent inspection verifies all three
retained source exports. Image SHA-256:
`6b014c2ac464bd1292b4d86cca9748ee234e786d0278f9468e2ff25f8d05b548`.
Existing diagnostic descriptors retain explicit source-symbol roots and dead-strip
new unreachable adapters whose callees are missing. This preserves their earlier
graphs without introducing fake dependencies. The full-game replacement coverage
gate remains blocked, with zero complete code/data/units and no runtime claim.

Synthetic tests check conservative selection, malformed/repeated metadata,
wrong relocation targets, deterministic generation, and executable host fixtures
that pass object pointers/arguments/return values through the source calls. Host
Clang is used only for those fixtures; real-game matches use pinned LLVM-GCC.

Final local validation passes **110 tests**, doctor, the 500-entry source validator,
and native baseline/head regression checks. The existing connected-gameplay link
also passes with retained source roots (image
`b7ec003fd99f178ca111f30123ccb497c6dd23c730962b33791d058153bed548`).
The retained 73-group image passes with hash
`1b7b5e3c9081d100cdf13b7cc400679b29d57f7a8d2cd9e1366c3546e96a2f46`;
614 retained source symbols survive inspection, comprising 590 earlier exact
functions and 24 differing candidates. None of this batch's new forwarders are
live in that older graph. Its comparison report also includes newly compiled,
unreachable source routines; those are not described as linked functions.
Hosted checks for the final head are recorded separately on PR #7.
