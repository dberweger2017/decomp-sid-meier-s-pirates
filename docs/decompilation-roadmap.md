# Decompilation roadmap

The target is a complete reconstructed game with verified source, data, ABI and
link layout. Exact function counts alone are a poor measure of that target.
The October 13 baseline has 1,613 exact functions out of 9,177 (17.5766%), but
only 18,124 out of 4,080,584 recorded function bytes (0.4442%). Of those exact
functions, 1,538 are at most 16 bytes. Data matching covers 2,236 bytes out of
3,827,236 (0.0584%). Replacement linking is still blocked. These are inventory
record measures; overlapping STABS ranges and ABI entry points retain their
record weights, so they are not an estimate of development effort.

## Recommended order

1. **Keep a trustworthy baseline and calibrate the compiler.** Keep source
   policy, shared-input/profile comparisons, build health and exact-match
   regression gates intact. Identify compiler differences using representative
   comparisons, copies, floating-point operations, calls and constructors from
   each original library. Time-box flag trials, record failed trials, and avoid
   per-function tricks that prevent completing the rest of an object group.
   Existing exact matches establish local evidence, not one proven compiler or
   flag set for all libraries. Spend approximately one investigation session
   on calibration for every three sessions on connected source recovery.

2. **Complete small foundational object groups.** Prioritize coherent bounds,
   matrix/vector, container and resource-reader routines that other code calls.
   Reconstruct fields, strides, signedness, ownership, virtual slots and data
   along with their methods. A partial header that matches a getter is not a
   usable class. Preserve the original translation units and unity groups.
   Keep correctly explained fuzzy candidates while pursuing exactness; source
   coverage, behavior checks and exact byte equality are separate outcomes.

3. **Follow dependencies into one usable subsystem.** Complete a vertical
   slice rather than selecting the smallest function across the entire game.
   Start with bounds transforms/camera visibility, then controller/keyframe
   interpolation and versioned sequence loading. Resolve their required
   globals, constants, RTTI/vtables and relocation targets in the same batch.
   Use generated boundary-case fixtures and bounded execution comparisons
   where practical, especially for parsers, floating-point math and lifetime
   operations. Link recovered subsets with the pinned linker to expose missing
   symbols and ABI assumptions; subset links do not earn full-game credit.

4. **Expand through game-specific unity groups.** The four Pirates unity
   groups hold about 2.36 MB of missing recorded function bytes at this
   baseline. Work through callers of the completed foundations: rendering and
   scenes, map/navigation, battles, UI and audio. Debug source-path attribution
   can point at headers containing many unrelated routines, so use original
   object groups and actual calls to choose work. Recover non-code data and
   initialization alongside code instead of leaving them to the end.

5. **Close runtime and whole-image gaps.** Reconstruct startup, platform
   integration, initialization/destruction order, imports and original section
   layout. Enable replacement linking when its coverage gates can pass, then
   validate the replacement's behavior and continue matching the remaining
   bodies. A successful link, behavioral equivalence and original byte equality
   each need their own evidence.

## Next three batches

| Batch | Concrete scope | Acceptance evidence |
|---|---|---|
| Bounds foundation | Seven `ISEAABBBox` bodies, observed layout and render-state scheduling | Historical compilation, host checks of merges/NaNs/saved extents, separate exact/fuzzy results, no lost exact matches |
| Bounds to visibility | `ISEAABBBox::transform` (1,404 original bytes), `ISECamera::IsVisible(ISEAABBBox&)` (520 bytes), and required matrix/plane helpers | Explained layouts/relocations, normal and boundary fixtures, cohort report and subset link |
| Animation and assets | `ControllerSequence`, keyframe channels, versioned sequence readers and their owned data | Version/bounds/lifetime fixtures, required globals/RTTI, historical cohort comparisons and subset link |

The original app advertises LLVM-GCC 4.2 / Xcode 4.3.2 / SDK 5.1, while linked
libraries may use other profiles. See [compiler evidence](toolchain.md) and the
[recovery ledger](recovery-loop.md) before adopting new flags. Use
`tools/cohort.py` to keep each batch's explicit inventory IDs and progress.

The foundation and bounds-to-visibility source checkpoints now pass their
acceptance checks. [Bounds-to-visibility evidence](bounds-camera.md) records
the partial layouts, new exact constructor, fuzzy bodies, 127 ARM fixtures and
diagnostic subset link. Those methods still need byte matching and complete
runtime integration. The next source batch is animation and assets; keep its
version, bounds and ownership dependencies together rather than accumulating
unconnected wrappers.

## Repeatable batch workflow

1. Select one original group or a small connected cohort and save its baseline
   report. Inspect original instructions, boundaries, calls and data accesses.
2. Write ordinary C/C++ with explicit evidence for observed fields and honest
   notes for unknown types. Add implemented IDs only for actual source bodies.
3. Compile affected objects with the historical compiler; compare every new
   function and every previously exact function affected by shared headers or
   flags. Keep a bounded list of compiler/source-shape trials.
4. Exercise behavior that byte comparison does not establish. Report exact
   functions/bytes, source coverage, fuzzy results, data and linking separately.
5. Run the full regression report, commit a reviewable batch, and require CI
   before merging. Update the cohort ledger and choose the next callers or
   missing dependencies exposed by that batch.

Stop counting new tiny wrappers as the main milestone. The more useful next
milestone is one complete, tested subsystem with its data and a diagnostic
link, while the exact-byte total continues to increase.
