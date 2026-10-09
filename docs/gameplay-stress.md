# Connected gameplay recovery experiment

This PR preserves the merged baseline of **411 exact functions / 4,864 code
bytes**, one exact four-byte data allocation, and zero complete replacement-link
units. Those exact functions have a median size of eight bytes and a maximum of
108 bytes. Larger-function matching efficiency is **not yet demonstrated**.

## Draft the cohort, measure, then refine groups

Following the requested workflow, select a bounded, connected subsystem, draft
all its selected routines, build them together and inspect the remaining
assembly differences by original compilation group. High-fuzzy candidates stay
visible as `different`; only fully resolved equal bytes earn `matched` credit.
The script automates selection, incremental building, reports and prioritization.
It does not automatically recover source or generate placeholder return bodies.

```
python tools/cohort.py select --name "Subsystem" --symbol-regex 'YourClass|HelperName' --output config/cohorts/subsystem.json
python tools/cohort.py build config/cohorts/world-map-projection.json --output build/cohorts/projection.json
python tools/cohort.py report config/cohorts/battle-grid.json --output build/cohorts/grid.json
```

`build` runs Ninja before reporting. `report` reads the saved native report; use
it after a build or to inspect a saved checkpoint. JSON and Markdown preserve
original groups, source presence, per-function statuses, exact bytes, and fuzzy
similarity. Groups sort by remaining size × (1 − similarity), a triage heuristic,
not an estimate of remaining effort. Inventory/report coverage loss fails rather
than shrinking the cohort silently.

Two fuzzy denominators remain visible: compared, resolved source candidates;
and all cohort bytes, with missing/unresolved work contributing zero. This
prevents one small high-similarity function from concealing a mostly missing
subsystem. Both values weight the existing instruction/operand/register/address
alignment score by original byte size. No new address masking is introduced.

## Source outcomes

The complete **world-map projection utility chain** has two source candidates:
`WMapXY_Shi(float,float,float,int,int,int*,int*)` calls `RemapPoint` twice, which
calls the real imported `modf`. This is a bounded utility cohort, not all world
navigation, map rendering or sailing simulation. The original exported name is
`WMapXY_Shi`, not `WMapXY_Shift`. The `bool` return type is provisional; the
observed return is one. `noinline` on `RemapPoint` is an investigation annotation
that preserves the original call boundary, not proof of its historical source.

| Function | Original / candidate bytes | Assembly similarity | Verified match |
|---|---:|---:|---|
| RemapPoint | 204 / 228 | 38.8889% | No |
| WMapXY_Shi | 240 / 228 | 44.4444% | No |
| BattleGrid::SetSquareEnable | 328 / 328 | 78.0488% | No |
| NiAVObject::GetProperty | 88 / 92 | 71.1111% | No |
| BattleGrid::SetGridEnable | 112 / 120 | 37.9310% | No |
| BattleGrid::SetLayerEnable | 80 / 80 | 95.0000% | No |

The projection cohort has **2/2 candidates**, with **41.8919%** byte-weighted
assembly similarity. The broader battle-grid display cohort keeps all 17
records visible: **4/17 candidates**, 71.8850% similarity among compared
candidates, but only **5.1883%** across its whole original footprint. Its larger
constructor, allocator, geometry generation and initialization paths are missing.

The 328-byte square update reconstructs layer bit fields, locked-square checks,
material alpha/revision updates, an indexed vertex-alpha loop and a geometry
dirty flag. Its real property lookup traverses a list and invokes an observed
virtual type slot. Class capacities, field names, inheritance and complete
layouts remain hypotheses. `ObservedType` and the preceding undefined virtual
slot declarations describe an ABI view; they are not recovered implementations.
Never instantiate these partial types. No fake dependency implementations, copied
original-byte bodies or assembly fallbacks are used.

Bounded trials examined optimization levels, frame-pointer/r9 settings,
scheduling, floating-point settings and ordinary source shapes. A frame-pointer
profile raised square similarity to 81.7073% but regressed three existing map
hash matches (84 bytes); it was rejected. The retained unit profile preserves
all earlier matches. Residual differences include compare direction, scheduling,
register allocation, float/double register choices, stack alignment and redundant
boolean selection. The 476-byte texture generator was inspected and drafted only
in ignored investigation files; it is not an active candidate and earns no
source credit. Compiler backend/flag equivalence remains unproven.

## Bounded behavior evidence

Install the optional hash-pinned [Unicorn ARM emulator](https://github.com/unicorn-engine/unicorn/tree/2.1.4):

```
python -m pip install --require-hashes -r requirements-emulation.txt
python tools/battle_grid_probe.py
python tools/world_map_probe.py
```

The historical candidate objects are relocated using the matching core before
execution. The battle-grid path passes **49/49** differential scenarios, including
layer/grid/square operation sequences, edge indices, locking, no-op transitions,
empty ranges, null list elements, found/missing property types and empty lists.
The projection chain passes **53/53**, including positive/negative rounding,
half-integer boundaries, varied scales and offsets.

These compare observable memory, return values, callback receivers and restored
SP against the original routines under identical synthetic memory. The square
candidate calls the candidate property lookup; the projection candidate calls
the candidate remapper. Only virtual type queries and imported libc `modf` are
explicitly modeled external operations. Projection candidates use isolated
emulation pages because a larger remapper would overlap the next original
function. Only already resolved ARM BL sites are rebased for this separate test
layout; matching still compares at original addresses. Unknown callees or
contradictory relocation metadata fail. Tests reject wrong callee effects,
nonreturning execution, and inconsistent verified status.

This is finite, bounded behavior evidence. It excludes invalid indices,
undefined conversions/divisions, absent required materials, ownership,
constructors, allocation and real iOS runtime behavior. Passing it grants **zero
exact-match and replacement-link credit**. It is not a proof of equivalence.

## Dependency linking

```
python tools/recovery_link.py config/diagnostic-links/gameplay-stress.json
```

The two original unity objects structurally link with real source callees and
SDK imports. Independent image inspection verifies `_modf` through
`/usr/lib/libSystem.B.dylib` and `___umodsi3` through `/usr/lib/libgcc_s.1.dylib`.
The image SHA-256 is
`432148e7e32f0bc108f141ada163259662ed598aa45328e2218bc8be22a0ac8f`.
An initial partial material hierarchy emitted unresolved placeholder vtable
entries; the corrected ABI view does not define an unrecovered virtual override
or invent those implementations. This diagnostic entry is a callable square
update, not app startup. Complete replacement code/data/units remain zero.

## Incremental timing

On this Apple Silicon host with the emulated historical Linux compiler, a no-op
Ninja run took 0.411 seconds. Comment edits to the BattleGrid source and included
header each recompiled only `PiratesIncludeCpp.o` and refreshed the full report
in 28.364 and 28.784 seconds, respectively. Restoring both preserved all earlier
exact matches. Isolated compiler probes generally took about 0.4–0.6 seconds;
the end-to-end loop includes SDK/input validation and global comparison/report
work. These are local observations, not throughput estimates for the game.
Batching related functions avoids paying this report cost after each draft.

## Validation and next refinement

All **103 tooling tests**, `doctor`, 102 bounded original/candidate execution
scenarios and the two-group dependency link pass locally. All 411 earlier code
matches and the four-byte data match remain exact, with no compile errors or
unresolved comparisons. Hosted CI runs the emulator's synthetic negative tests
on Linux/macOS, reports both cohorts, runs the real bounded execution checks and
retains source-only link diagnostics. Inputs, original code, SDKs, objects and
images are excluded from uploads and Git.

The source-first batch workflow is now reproducible, but the two-function
projection chain is only a starting stress cohort and its fuzzy score is low.
Next refine its float/double conversion and scheduling patterns, then expand a
cohort into substantive gameplay logic once compiler fidelity is better understood.
Keep the larger battle-grid case as a regression and performance stress target.
Do not interpret these results as efficient recovery of an entire gameplay system
or progress toward a runnable replacement image.
