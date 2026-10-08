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

An included UicButton header edit rebuilds only PiratesIncludeCpp4, preserving
all 236 verified functions and npos. The editor API exposes all 40 contributing
sources/headers in that unit, and the same selected function comparison remains
exact. The live server/API check passes; T3 preview snapshot/evaluation/navigation
time out at this checkpoint, so a fresh visual browser pass is not claimed.

## Second hundred-function batch

The next batch drafts 100 functions in 44 original object groups, including
engine/game unity units, multiplayer UI, Gamebryo and namespaced ISE/Phono2
libraries. **98 verify at 904 code bytes**, reaching **334 exact functions /
3,804 code bytes** (318 functions / 3,176 bytes beyond main). Exact code is
0.0932% of recovered function ranges; exact records are 3.6395%. Nine candidates
differ, 8,834 records remain missing, and there are zero compile errors, unresolved
comparisons or regressions of prior function/data matches. Detailed evidence is
retained in ignored `build/recovery/batch2-*` outputs. Each original group has its
own commit; the two failing candidates have explicit nonexact commits.

These are primarily actual field reads/writes, indexed object-group/node access,
ushort vertex/triangle counts, shared emitter-controller fields, camera/texture
flags, audio pointers and property-type constants. The small mutex/log
constructors verify only their observed flag initialization; neither establishes
a complete, instantiable object layout. Nine address-return methods were deferred
until their in-place/base-subobject layouts can be recovered.

Nested target identities have additional call-site evidence: FAnimation's
GetKFMTool constructs NiKFMTool and Initialize stores its result at +0x58;
UicDanceStep constructs a UicDanceHalo before storing +0x54; DanceUIScene
InitUIScene constructs UicLabel before storing +0xac; the draw-list render unit's
DrawUnit calls ISEDrawList::Draw with its +0x10 pointer; TriStrip's constructor
accepts VertexBuffer and stores the pointer at +0xc8. Other unencoded return
types, names, signedness and full virtual layouts remain hypotheses. Header-only
views earn no extra function/data progress.

ISEParticles::SetModelData remains different at 57.1429%: the historical
compiler emits additional conditional returns (16 versus 12 bytes).
ISETexture::IsAlphaEnabled also stays at 57.1429%, using a shift-and-mask
sequence instead of UBFX (16 versus 12 bytes). Optimization, scheduling, CPU,
control-flow and ordinary source-shape trials did not verify either; isolated
bitfield views narrow the load to 16 bits and also differ. The live bodies keep
the straightforward nullable setter and unsigned bit test, with no assembly
fallback, artificial access or exact-match credit.

The fifty-seven-group `recovery-fields.json` subset structurally links with image
SHA-256 `3ab798bd759504b0400a2785734b873be1dd3b16768899803df4c93a4566016c`.
Its comparison report has 303 exact functions / 2,720 code bytes, two nonexact
candidates and the four-byte npos allocation. Five selected shell callbacks
were already in main; baseline matrix/vector groups and the Phono facade with
missing call targets are excluded. Structural inspection passes with zero
replacement completion and unverified iOS runtime behavior. Reproduce with
`python tools/recovery_link.py config/diagnostic-links/recovery-fields.json`.

The pushed 236-function checkpoint passes Linux/macOS tooling and historical
candidate CI. Its native Linux artifact agrees with the saved macOS report for
all 9,177 functions, 5,056 data records and progress/coverage metrics; compiler
profile metadata remains separate. The pushed 334-function checkpoint also
passes both CI runs, with matching Linux/macOS function/data records and
progress/coverage metrics. The recovery goal continues.

## Shared-container batch

The third batch investigates **102 functions across 18 original groups**.
**77 verify at 1,060 bytes**, reaching **411 exact functions / 4,864 code
bytes** (0.1192% of function bytes; 4.4786% of records). There are 34 differing
candidates and 8,732 missing functions, with zero compile errors, unresolved
comparisons or regressions of prior exact function/data matches. All 92 tooling
tests pass. There are 67 configured original objects. The shared scaffolding
and each original group have separate commits; ignored `batch3-*` outputs retain
the detailed evidence.

Partial NiTMapBase/NiTMapItem templates recover unsigned key hashing, direct
key/value assignment and observed empty ClearValue hooks. Instantiations remain
in their original engine/game unity or PrintString objects. Key comparisons use
pointer/value identity, including char const* keys. Smart-pointer assignments,
destructors, allocator operations, complete layouts and virtual slots are not
recovered, and the partial types must not be instantiated. Other verified bodies
include RTTI/vector initialization, vector assignment, enum state access,
particle-count clamping, packed version construction and emitter declination.
The particle clamp matches after expressing the conditional update explicitly.

The compiler emits unsigned remainder through `__umodsi3`. Its original ARM
stub at 0x3e7bc0 is established by LC_DYSYMTAB, a supported stub encoding and the
referenced lazy-pointer entry, rather than an arbitrary address map. Seven new
synthetic tests reject wrong imports, duplicate stubs, nonzero branch addends,
unsupported encodings, malformed tables and misuse as function-pointer values.
The original function inventory remains identical. Reloading the development
server activated the changed matching core; subsequent source edits use the
ordinary incremental watcher. The workbench API shows the exact hash helper and
its resolved import. T3 preview opens but snapshots still time out, so this
checkpoint does not claim a new visual browser pass.

Twenty-two equality helpers and two audio validity predicates remain at 75%
similarity because MOV/CMP ordering differs; NiAccumulator::StartAccumulating
also remains different. Bounded optimization, pre/post-register-allocation
scheduling and conditional-source trials did not establish exact bytes. These
25 new differences receive no verified matching credit.

The sixty-two-group `recovery-containers.json` diagnostic subset structurally
links **380 exact functions / 3,780 bytes**, 27 differing candidates and npos.
Image SHA-256:
`a1503e8ff803e5a97c588c3f865bc6c9ce364e6cefc7f094d73383860732afe5`.
The initial link exposed a missing libgcc_s.1 dependency. The original undefined
symbol's library ordinal 12 identifies /usr/lib/libgcc_s.1.dylib; the SDK supplies
it. The corrected image's independent dyld inspection confirms that binding,
and the descriptor now requires it. Full-game replacement completion remains
zero and iOS runtime behavior remains unverified. Reproduce with
`python tools/recovery_link.py config/diagnostic-links/recovery-containers.json`.

Checkpoint 334 passed hosted CI with every function/data record and matching
metric agreeing across hosts. This new 411-function checkpoint needs its own CI.
The recovery goal continues in batches.

## Connected source-first cohorts

PR #4 investigates larger connected routines, retains fuzzy candidates, and adds
`tools/cohort.py` for a whole selected source pass followed by group refinement.
See [the experiment and measured limitations](gameplay-stress.md). The 411 exact
functions remain preserved; bounded behavior checks and diagnostic links earn no
additional exact or complete replacement credit.

## Two hundred additional release hooks

The next requested batch adds **200 exact functions / 976 code bytes**, reaching
**611 exact functions / 5,840 code bytes** across **102 configured original
objects**. All earlier 411 exact functions and the four-byte data allocation are
preserved. There are 40 differing candidates, 8,526 missing functions, zero
compile errors and zero unresolved comparisons. Inventory coverage remains
9,177 named records in 268 original groups. Exact function coverage is 6.6579%
by count and 0.1431% by original code bytes.

This batch contains 156 observed no-op release hooks and 44 constant-return
functions, committed separately in their 51 original groups. These bodies were
selected from original instructions, rather than added as stubs for absent
implementations. The hooks cover UI releases, controller lifecycle callbacks,
particle/animation operations, disabled platform utilities and shader setters.
They reproduce the behavior of this shipped binary, not a complete implementation
of the corresponding APIs on other platforms. No constructors, destructors,
address-return methods or non-virtual thunks are counted in this batch.

Partial interfaces, opaque parameter types and provisional result declarations
support ordinary C++ compilation. Symbol mangling establishes parameter names
and types, but does not establish return types, class layouts, static-versus-
instance ownership for unused parameters, or complete virtual hierarchies.
Matching these tiny bodies cannot settle those source-level uncertainties.
Do not instantiate the partial types. Original compiler flags remain unproven.
No assembly or original-byte payloads appear in candidate source.

Reproduce the selected comparison ledger with:

```sh
python tools/cohort.py build config/cohorts/release-hooks.json --output build/cohorts/release-hooks.json
python tools/recovery_link.py config/diagnostic-links/recovery-release-hooks.json
```

The native report, the cohort ledger and objdiff adapter expose exact matching
separately from assembly similarity and replacement linking. Full-game linking
remains at zero completed code/data/units, and iOS runtime behavior is unverified.
The local tooling suite passes 104 tests. Hosted CI must validate this new head;
the earlier PR #4 historical failure was traced to nested staging falling back
to the generic linker template, and a regression test now covers preserving the
validated profile and its proof.

The 51-group `recovery-release-hooks.json` diagnostic subset structurally links
369 exact functions / 2,532 code bytes and 21 differing candidates. Independent
image inspection passes with SHA-256
`196801ea118306dd6078bf051cd588fc3a38698b62d8e4b30423efbbbdecba95`.
It includes previously recovered contributors in selected unity groups; the
subset's 369 exact functions are not 369 new matches. Completed replacement
code/data/units remain zero, and runtime behavior remains unverified.

## Two hundred more tiny shipped bodies

The following batch adds **200 exact functions / 1,052 code bytes**, reaching
**811 exact functions / 6,892 code bytes**, with all earlier 611 function matches
and the four-byte data match preserved. There are 40 differing candidates,
8,326 missing functions, no compilation errors and no unresolved comparisons.
All 9,177 original function records and 268 groups remain covered. Definitions
were committed separately in their **31 original compilation groups**.

The new bodies comprise **113 shipped fail-fast routines**, **48 XML type
queries**, and **39 constant-return functions**. The fail-fast routines include
factory, animation/controller and collision entry points whose complete original
bodies are a trap instruction. Ordinary C++ `__builtin_trap()` reproduces the
historical compiler's exact instruction, verified first with an isolated factory
probe and then with each configured function. This restores their observed
failure behavior; it does not provide working implementations of those engine
operations. No additional fail-fast fallback is supplied for other missing code.

The XML queries include 24 null-returning base queries and 24 self-returning
specialized queries across the distinct TiXml and ISEXml object families. The
parser and other XML operations remain unrecovered. The remaining constants
cover shipped UI releases, audio compatibility paths, rendering counters and
animation-key allocation paths. Output-reference parameters in constant-return
methods remain untouched where the original instruction bodies leave them alone.

Source-level results and unused static-versus-instance ownership remain explicit
hypotheses where mangling or a tiny body cannot establish them. All partial
interfaces remain unsuitable for instantiation; no complete layout, hierarchy,
virtual table, allocator or linking/runtime completion is claimed.

```sh
python tools/cohort.py build config/cohorts/tiny-bodies.json --output build/cohorts/tiny-bodies.json
python tools/recovery_link.py config/diagnostic-links/recovery-tiny-bodies.json
```

The local tooling suite again passes **104 tests**. The preceding 611-function
checkpoint passed hosted Linux/macOS tooling and historical compilation/linking
CI; this 811-function head requires its own hosted run. Original game/SDK inputs
and generated code images remain excluded from Git and CI artifacts.

The expanded 73-group `recovery-tiny-bodies.json` subset structurally links
590 total exact functions / 3,780 code bytes and 24 differing candidates.
Independent image inspection passes with SHA-256
`f6f836365b8c8271e245b674c0bf2fb17f4258d607a99ded188b0ab678e6b64b`.
Those totals include earlier contributors in selected groups, rather than 590
new matches. Replacement completion remains zero and runtime behavior is unverified.

## Additional 500-function ABI-entry batch

The next requested batch verifies **500 additional functions / 8,000 bytes**,
reaching **1,311 exact functions / 14,892 bytes**. The earlier 811 functions and
four-byte data allocation remain exact, with zero regressions, compile failures
or unresolved comparisons. Inventory retains all 9,177 records / 268 groups.
There are 40 differing candidates, 7,826 missing records and 153 configured units.

The 92 separately committed original groups contain 255 complete-destructor
wrappers, 233 method forwarders, six free-function forwarders and six
complete-constructor wrappers. Their source consists of ordinary C++ calls with
explicit ABI symbol declaration labels. There are no instruction/byte bodies;
larger callee implementations and complete class models remain independent.
This provides small-function byte progress and caller/callee structure rather
than completing 500 gameplay algorithms. Native reports identify the recovery
kind. See [the batch's method, scope and validation](small-function-batch.md).

A three-function recovered particle LinkObject chain structurally links from
source, with image SHA-256
`6b014c2ac464bd1292b4d86cca9748ee234e786d0278f9468e2ff25f8d05b548`.
Explicit live-source roots plus dead stripping keep older diagnostic graphs
reproducible without supplying fake missing callees. Replacement linking stays
at zero; the original image and iOS runtime remain unverified.

## Short ISE accessors and light setters

Nine short ISE accessors now verify exactly, adding 72 bytes across the existing
ISECamera, ISEEditableMesh, ISEInputBlk, ISENode and TriStrip object groups. Each original
body is a single ARM `add` of `this` with a fixed offset followed by `bx lr`:
ISECameraMgr's orthogonal-camera view uses `+4`; ISEInputBlk's touch-point view
uses `+0xa4`; ISENode's name view uses `+4`;
ISEEditableMesh returns its render-unit and material views at `+0xb4` and
`+0x130`, and its material thunk adds `+0x7c`; TriStrip returns its render-unit
and material views at `+0xb4` and `+0xd4`, and its material thunk adds `+0x20`.
The source uses ordinary pointer arithmetic. Return types are not
encoded in the symbols, so the partial declarations use `void*`; the complete
types and object hierarchies remain unrecovered.

Four more short ISE field-path functions now verify exactly, adding 48 bytes.
They recover the observed load-and-adjust sequences for ISEParticles and its
material thunk, ISEEntityRenderUnit::GetMaterial, and the TriStrip
GetVertexNum thunk. The source records only the pointer fields and offsets used
by these bodies; the containing class layouts remain partial.

Three ISELight setters have readable source candidates but remain different at
25% assembly similarity. The original stores `(r, g, b, 0)` at `+8` and
`+0x18`, and `(x, y, z, 1)` at `+0x38`. The candidates express those four
components with partial `ISELight` fields. The backend allocates the constant
and destination registers in the opposite order from the original; these three
functions receive no exact-match credit. This is a source-level result, not a
claim that the partial ISELight layout is complete.

The next short-function pass adds exact source for FFontString::SetPosition and
UicDanceStep::cleanStates (32 bytes). It also keeps three grounded candidates
visible: ISEEntity::GetRenderUnit uses the observed render-unit array at `+0xfc`
with a 24-byte stride and compares at 50%; UicFire's left/right cannon setters
write their observed fields and conditionally update the second value for
`cannonType >= 1`, comparing at 44.4444% each. The ISEEntity candidate emits
an `mla` for its scale while the original uses shifted adds. UicFire candidates
use a branch around the conditional write while the original uses ARM predicated
store instructions. These are source candidates without exact-match credit.

Three more small routines now verify exactly: UicMoveLabel::startMove stores the
input byte at `+0x94` and sets `+0x9c` to one; NiMemStream::Str sets its access
flag at `+0x15` and returns the string pointer at `+4`; Phono2::PThread's
constructor initializes its first two words to zero and ten. These add 48 bytes.

Four more short functions verify exactly: UicKeyboard::SetTextFont writes the
font pointer through the owner at `+0x54` to `+0x7c`, then to `+0x78`;
ShipBattleUIScene::SetShipInfo stores its two arguments in the ship-info view at
`+0xec` and `+0xf0`; UicCSB::ShowMyShipTxt clears the word at `+0x18b4`; and
Remark returns one while preserving the two stack-passed varargs registers.
These add 64 bytes.

UicDots::SetNextDot and SetPrevDot now have grounded candidates for the state
updates at `+0x78` and `+0x7c`. They remain fuzzy at 82.3529% and 57.1429%
assembly similarity because the compiler emits different predicated branches
and stores. They move two functions from missing to differing without exact
match credit. UicFire::GetCommandKey now verifies exactly: it returns `0x20`
when the state at `+0x50` is one and zero otherwise. UicCSBLand::Reset also
verifies exactly, clearing byte `+0x30`, setting byte `+0x70`, and clearing the
words at `+0x78` and `+0x7c`.

UicComboButton::GetCommandKey now has a source candidate that returns the
command key when the state at `+0xdc` is one or two, and zero otherwise. It
compares at 61.5385% similarity; the compiler places the conditional result in
a different register and uses a different conditional move. It remains a
fuzzy candidate without exact-match credit.

The resulting report has **1,598 exact functions / 17,836 bytes**, 49 differing
candidates, 7,530 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

The next short-function batch adds four exact bodies: FSound::IsLooping reads
the word at `+0x1c` as a boolean; FSound::GetShortCircuitScriptField tests the
mask at `+0x60`; Heap_FromBlock reads the owner pointer from the 24-byte block
header immediately before the allocation; and FAudioMemMgr's constructor
clears its first word. The heap accessor uses an empty register constraint to
preserve the original separate subtract-and-load instruction sequence.

The updated report has **1,602 exact functions / 17,900 bytes**, 49 differing
candidates, 7,526 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Two more constructors verify exactly: ISEVector3's default constructor clears
the three words at `+0`, `+4`, and `+8`; CFIleIO's constructor clears its first
three state words. UicFrameAnimation::Reset adds a grounded fuzzy candidate for
the conditional byte clear at `+0x6d` and word clears at `+0x64` and `+0x68`;
it compares at 82.3529% because the candidate rematerializes the zero register
before each word store.

The latest report has **1,604 exact functions / 17,940 bytes**, 50 differing
candidates, 7,523 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

CPVRTModelPOD's constructor now verifies exactly, clearing its observed word at
`+0x54`. MacMoviePlayer's constructor has a candidate that stores a non-null
controller pointer; it remains fuzzy at 57.1429% because the candidate emits a
branch around the conditional store while the original uses predication.

The newest report has **1,605 exact functions / 17,952 bytes**, 51 differing
candidates, 7,521 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

CPVRTString copy assignment now forwards the source buffer and length to the
existing `assign` method. Its relocation resolves, but it compares at 44.4444%
because the candidate emits a normal call and stack frame instead of the
original tail branch.

The current report remains at **1,605 exact functions / 17,952 bytes**, with 52
differing candidates, 7,520 missing functions and no compile errors or
unresolved comparisons. Full-game replacement linking remains at zero.

Four direction wrappers now call the already matched scene methods for
`clearDir` and `IsChooseDir`, using the observed scene pointers at `+0x18` and
`+0x28` on the two chosen-state objects. All four calls resolve and compare at
80% similarity; the compiler places the receiver load before the frame-pointer
move, unlike the original instruction order.

The updated report remains at **1,605 exact functions / 17,952 bytes**, with 56
differing candidates, 7,516 missing functions and no compile errors or
unresolved comparisons. Full-game replacement linking remains at zero.

UicMap::GoToCityReport now matches exactly. The 24-byte method stores the city
report mode value `6` at `+0xb0`, the city index at `+0x100`, and clears the
word at `+0x104`.

The latest report has **1,606 exact functions / 17,976 bytes**, 56 differing
candidates, 7,515 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Two thread-exit methods now verify exactly in the existing
`PiratesIncludeCpp4.o` unity group. `ISEThread::Exit` sets the observed byte at
`+8` and calls the resolved `_pthread_exit` import with a null value;
`CPiratesLoading::ExitThread` forwards its receiver to that method. Their 24-
and 12-byte bodies match the original instructions and relocation targets.

The latest report has **1,608 exact functions / 18,012 bytes**, 56 differing
candidates, 7,513 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

`FAudioLibFactory::GetAudioMgr` now has a source candidate returning the
observed `_gs_oAudioMgr` global. Its relocation resolves to the 8-byte common
allocation in the same original group; the candidate compares at 50% because it
uses a literal-pool offset instead of the original `movw`/`movt` pair. This is
a fuzzy function candidate, not an exact match, and the global data remains
unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 57 differing
candidates, 7,512 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

`NiImageConverter::GetImageConverter` now returns the observed
`ms_spConverter` singleton from its 8-byte common allocation in the same
original unity group. The relocation resolves; the candidate compares at 50%
because it uses a literal-pool load instead of the original `movw`/`movt`
address sequence. The singleton data itself remains unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 58 differing
candidates, 7,511 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Five `Ni*::GetRTTI` methods in the existing `TempIncludeCpp3.o` unity group
now return their matching 8-byte `m_RTTI` common allocations. Each relocation
resolves to the expected class symbol; the candidates compare at 50% because
they use a literal-pool load instead of the original `movw`/`movt` sequence.
The `m_RTTI` allocations themselves remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 63 differing
candidates, 7,506 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Five more RTTI getters in the same object now return the class-local
`m_RTTI` allocations for `NiGeometry`, `NiMaterialProperty`,
`NiBackToFrontAccumulator`, `NiAlphaAccumulator` and `NiAlphaProperty`. All
five relocations resolve to the expected data symbols and compare at 50%; the
allocation contents remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 68 differing
candidates, 7,501 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Ten additional `Ni*::GetRTTI` methods now return the verified class-local
`m_RTTI` common allocations. Their relocations all resolve to the matching
class symbols and each compares at 50%; the allocation contents remain
unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 78 differing
candidates, 7,491 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Ten further `Ni*::GetRTTI` methods in `TempIncludeCpp3.o` now return their
matching class-local common allocations. All ten relocations resolve; each
candidate compares at 50%, and the allocation contents remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 88 differing
candidates, 7,481 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Ten `Ni*::GetRTTI` methods in the `TempIncludeCpp4.o` unity group now return
their class-local `m_RTTI` allocations. All ten relocations resolve to the
matching data symbols and compare at 50%; the data contents remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 98 differing
candidates, 7,471 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Ten more `Ni*::GetRTTI` methods in `TempIncludeCpp4.o` now return their
matching class-local allocations. Each relocation resolves to the expected
symbol and compares at 50%; their allocation contents remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 108 differing
candidates, 7,461 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.

Ten more RTTI getters in `TempIncludeCpp4.o` now return their class-local
allocations. Their relocations all resolve to the corresponding `m_RTTI`
symbols; each compares at 50% and the data contents remain unmatched.

The latest report has **1,608 exact functions / 18,012 bytes**, 118 differing
candidates, 7,451 missing functions and no compile errors or unresolved
comparisons. Full-game replacement linking remains at zero.
