# Continuous source recovery

Work branch: `feat/easy-function-recovery`. Starting baseline: 16 verified
functions / 628 bytes, no matched data and no completed replacement-link units.

Start with GameAudio's original small release-build entry points, then PowerVR
string/resource accessors. Their declarations live in separate original object
groups. Empty scaffolds have empty `implemented_functions` lists and earn no
matching progress. Unity groups retain their original grouping; see
`src/game/unity/README.md`.

For each function:

1. Inspect its original instructions, symbol, boundary and any relocations.
2. Recover a readable C++ definition, recording uncertain ABI/type assumptions.
3. Add only that function to its group's explicit implemented list.
4. Rebuild with the validated historical compiler. Inspect the terminal diff and
   relocation-resolved byte verification; similarity alone never accepts a match.
5. Check all earlier matches and compilation health, then commit the verified
   function immediately. Push small batches to the recovery PR.

Store detailed comparison/flag evidence under ignored `build/recovery/`. Git
commits, the live native report and PR updates provide the progress ledger.
Return to difficult functions after easier candidates; do not change unit flags
in a way that loses an existing match.

Periodically link an isolated set of recovered source objects with the pinned
linker. Record structural diagnostics and image hashes. Such subset links earn
zero full-game completion and do not claim iOS runtime behavior. Re-run the full
replacement coverage gate periodically to make remaining blockers visible.

After recovering GameAudio, reproduce its subset check with
`python tools/recovery_link.py config/diagnostic-links/gameaudio.json`.
This runs the same historical compiler, Ninja, pinned linker and independent
image inspector in `build/recovery-link/workspace`, retaining logs and reports
without changing the active candidate manifest or replacement-link state.
The direct function entry establishes structural linking only, not process startup.

The original GameAudio unit contains eight `bx lr` entry points and an initializer
returning zero. Recovering those observed no-op bodies describes this shipped
release build; it does not restore another audio implementation. Enum identifiers
and Init's source return type remain hypotheses even when emitted bytes match.
PowerVR class declarations are partial ABI reconstructions, not complete recovered
types or allocator implementations.

## First checkpoint

All nine GameAudio entry points verify at 40 bytes, raising the baseline to 25
functions / 668 bytes. Each recovered function has its own verified commit. Its
isolated diagnostic link passes with image SHA-256
`5ad5b00e850fffae58dc4a93d5e12548e2962249796305d53fe520dc0087e4c3`.
Root replacement linking remains blocked with zero completed units.

The PowerVR accessor and FSound batch raises progress to 54 functions / 968 bytes,
preserving every earlier match. All 85 tooling tests pass. The four-group subset
descriptor `config/diagnostic-links/core-accessors.json` structurally links with
image SHA-256 `adbbd43abc9db3cb37e586c4afdbeae089eee9f616a23de0f487385dafd6f6c7`.
Constructors/destructors, larger methods, class hierarchies and associated data
remain missing; the diagnostic image adds no full-game completion.

## Audio accessor checkpoint

All 21 selected FSound3D methods verify, reaching 75 functions / 1,188 code
bytes. The browser watcher performed each rebuild; every verified function has
its own commit. CPVRTString::npos also verifies as one 4-byte data allocation,
with explicit observed __TEXT,__const placement and alignment. Default literal4
emission was correctly rejected; original source attributes remain unknown.

The five-group `audio-accessors.json` diagnostic descriptor links with image
SHA-256 `89f3ae1c6f84ca0f20701b7fbd8f2278cbef6135eaf8bf29f26b4f7f35ed8556`.
It includes the reconstructed sentinel and both sound classes while retaining
zero replacement completion and unverified runtime behavior. Larger methods,
constructors/destructors and complete class hierarchies remain unrecovered.
Next targets are the small FAudioManager methods.

## Manager accessor checkpoint

Sixteen FAudioManager accessors and pointer/count setters verify, reaching 91
functions / 1,332 code bytes: 75 functions / 704 bytes beyond the main baseline.
Each function has its own commit and the browser watcher rebuilt each candidate.
Every previous verified function and the 4-byte npos allocation remain matched;
there are no compile errors or unresolved comparisons. Five earlier candidates
still differ, and 9,081 recovered functions have no source candidate.

The preceding 75-function pushed checkpoint passed macOS/Linux synthetic tooling
and historical candidate CI. The manager batch requires its own CI run.
Full-game replacement linking remains blocked at zero completed units; the
five-group diagnostic link remains the latest subset-link checkpoint. Conditional
manager setters and additional small original audio methods are next.

The six-group `audio-manager.json` subset now structurally links with image
SHA-256 `7bea096115c654dceccb95630b18e5720fc3ab5b4540694b7d2905e151276070`.
Its 75 selected functions and npos still verify, with zero replacement credit.
The first conditional manager setter compiles but differs (40 generated bytes
versus 36 original); isolated optimization, CPU and ordinary source-shape trials
did not resolve it. The attempted body is retained only in ignored investigation
outputs, earns no progress, and can be revisited after easier functions.

## Shared-sound checkpoint

Eight FSharedSoundData methods, two FAudioSystem release-build methods and
Heap_Dump verify, reaching 102 functions / 1,472 code bytes. This includes the
32-entry buffer-size summation loop and instance-count update, reconstructed in
ordinary C++. Original RTTI/vtable establishes the shared-sound base and slot
order; unencoded return types and the initialization payload remain provisional.

The attempted heap header/bounds checks compile but differ in negative-address
folding and control flow. They remain missing in the active manifest; ignored
source queues and detailed comparisons retain the investigation evidence.
The 91-function CI checkpoint passed after the repository became public and its
billing-blocked PR workflow was retried. New checkpoints need their own CI.
No original inputs or SDK files are tracked; replacement completion remains zero.

The nine-group `recovery-core.json` descriptor now links all 86 newly recovered
functions (844 code bytes) and npos, excluding the original 16-function baseline
and unimplemented Phono2 targets. Its image SHA-256 is
`8fb6a60d06e2fa0ddd1cc0385579d9fa84311f536a6c08dbbdb9a27f2fe8bbf9`.
Structural inspection passes; replacement credit and runtime validation stay zero.

## Phono call checkpoint

The facade now has sixteen exact call wrappers, reaching 118 verified functions
/ 1,944 code bytes. Each wrapper's external branch relocations resolve to named
original Phono2 functions before byte equality is checked. Isolated trials found
`-mllvm -post-RA-scheduler=false` necessary to keep frame-pointer setup in the
observed position; this investigation profile does not prove original flags.

SetDopplerFactor remains a readable source candidate at 88.8889% assembly
similarity with both correct relocation targets. MOVT and a register move occur
in the opposite order, so it earns no verified-function or matched-byte credit.
Keeping this difference visible provides a further scheduling investigation
without changing unit flags in a way that loses the sixteen exact wrappers.

The pushed 102-function checkpoint passed both Linux/macOS tooling and historical
candidate CI. All 9,177 function records, 5,056 data records and matching/coverage
metrics in its Linux report equal the saved local macOS report; compiler profile
metadata remains explicit. The nine-group structural link still earns zero
replacement completion.
