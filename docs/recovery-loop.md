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
