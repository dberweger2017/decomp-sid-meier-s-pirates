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

The original GameAudio unit contains eight `bx lr` entry points and an initializer
returning zero. Recovering those observed no-op bodies describes this shipped
release build; it does not restore another audio implementation. Enum identifiers
and Init's source return type remain hypotheses even when emitted bytes match.
PowerVR class declarations are partial ABI reconstructions, not complete recovered
types or allocator implementations.
