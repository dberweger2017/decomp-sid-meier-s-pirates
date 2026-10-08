# Continuous source recovery

Work branch: `feat/easy-function-recovery`. Starting baseline: 16 verified
functions / 628 bytes, no matched data and no completed replacement-link units.

Start with GameAudio's original small release-build entry points, then PowerVR
string/resource accessors. Their declarations live in separate original object
groups. Empty scaffolds have empty `implemented_functions` lists and earn no
matching progress. Unity groups retain their original grouping; see
`src/game/unity/README.md`.

Recovery now works in batches of about 100 easy functions, following the user's
request to reduce repeated edit/build overhead. Inspect each selected function,
but draft related definitions and partial headers together within the original
compilation groups. Update their explicit implemented lists together, let Ninja
compile each affected object, then compare every function in the batch.

1. Inspect original instructions, names, boundaries and relocation targets.
2. Draft ordinary C++ bodies and observed fields; document uncertain types.
3. Preserve original unity groups and include contributing source files from
   their corresponding unity translation unit.
4. Use the browser watcher to rebuild the batch with historical LLVM-GCC.
   Verify relocation-resolved bytes for each function. Similarity earns no
   verified-match credit; unresolved cases remain unresolved.
5. Check every earlier verified function/data match and compilation health.
   Iterate on failed functions and flags without losing existing matches.
6. Commit passing original groups frequently, retaining the verified IDs in
   commit messages, and push the checkpoint to the recovery PR.

A batch size is an investigation queue, not a requirement to make every function
match before committing useful results. Header/source edits within an existing
configured unit remain incremental. Adding units changes configuration and may
rebuild the configured objects; batching additions avoids repeating that work.

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

CloseStream also remains different at 88.8889%, with both calls resolved but the
compare and frame-pointer setup in opposite order. It is a source candidate,
not a verified match. These two Phono candidates demonstrate the browser's
separate similarity/byte-verification categories on real recovered call code.

Four remaining small Phono compatibility methods verify at 24 additional bytes,
reaching 122 functions / 1,968 code bytes (106 functions / 1,340 bytes beyond
main). The shipped startup returns success, DirectSound accessor returns null,
and distance/rolloff setters are no-ops; their source-level return type/pointee
assumptions remain documented. Each exact function has its own commit.

The 116-function pushed checkpoint passed Linux/macOS tooling and historical
candidate CI on the public repository. At checkpoint 122, all earlier verified
function/data matches remain intact, seven source candidates differ, and there
are no compile errors or unresolved comparisons. The newest pushed checkpoint
requires its own CI run. Source-subset linking remains diagnostic, with zero
full-game completed units. Further small audio methods and ABI scaffolding are
next; the recovery goal remains active.

## First hundred-function batch

Six further FSound3D accessors and six FKnob methods plus two FSoundScape getters
raised the checkpoint from 122 to 136 verified functions / 2,240 code bytes.
The FSound3D cone output needed the per-unit `-arm-reserve-r9` investigation
flag, preserving all other methods. Recursive FKnob volume multiplication remains
missing after an isolated 92.8571% trial; its operand order does not verify.

The first 100-function batch verifies all 100 definitions at 660 code bytes in
one watcher build. Total progress reaches **236 functions / 2,900 code bytes**,
with 220 functions / 2,272 bytes beyond main's original baseline. Every earlier
function/data match is retained; seven candidates differ, 8,934 functions remain
missing, and there are zero compile errors or unresolved comparisons. The original
9,177 records / 268 groups remain intact. The 122-function Linux/macOS and
historical candidate CI checkpoint passed; this newer checkpoint needs its own CI.

The five original unity groups are committed separately: FireIncludeCpp (23
functions), FireIncludeCpp2 (10), PiratesIncludeCpp2 (28), PiratesIncludeCpp4
(38), and PiratesIncludeCpp3 (1). Their included sources live under fireplace,
game UI/world and Gamebryo directories. The batch covers typed field access,
indexed animation/child access, boolean state resets, observed constant returns
and empty release callbacks. Empty bodies describe original shipped instructions;
partial headers are not safe to instantiate as complete recovered objects.
Unknown hierarchy, virtual slots, object sizes, unencoded return types, enum
values and signedness remain explicit. The check-box trailing animation extent
uses a GNU zero-length declaration until its real size is recovered.

Detailed comparisons and before/after reports are retained under ignored
`build/recovery/batch-100-*`; the manifest and five commits identify every
verified function. No original-byte or assembly bodies are used. RTTI getters
were deferred after a probe exposed Darwin global-address lowering differences
in the current historical backend; see [compiler limitations](toolchain.md).

The sixteen-group `recovery-unity.json` diagnostic descriptor structurally links
all 200 newly recovered functions outside the baseline/Phono facade (1,776 code
bytes) and the four-byte npos allocation. Image SHA-256:
`3bd6fa5feba15291ec494f7dc857df69d50f40209c430dcbc36ce0686397069b`.
The independent image inspector passes, with zero replacement completion and
unverified iOS runtime behavior. Reproduce it with
`python tools/recovery_link.py config/diagnostic-links/recovery-unity.json`.
