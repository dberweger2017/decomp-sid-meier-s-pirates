# Progress, browser and linking implementation plan

## Starting point

The workbench already exports whole-inventory fuzzy assembly similarity and
verified matched-code/function counts separately. Its fuzzy score is our own
instruction/operand comparison, not stock objdiff's scoring algorithm. The
browser currently shows candidate-only similarity and verified byte/function
counts. Non-code data matching and full-game linking are not implemented.

The current code denominator is the union of recovered STABS function ranges,
not every executable section byte. Preserve that scope explicitly until code
outside those ranges is inventoried. Unrecovered source stays missing; original
bytes and disassembly views never count as recovered source.

## 1. Make existing progress measures clear in the browser

Expose these separate measures in native reports, CI summaries and the browser:

| Measure | Definition | Missing or unsupported work |
|---|---|---|
| Overall fuzzy code | Function-size-weighted assembly similarity across the inventory | Missing source contributes zero; unresolved comparisons remain explicitly unresolved |
| Exact matched code | Union of complete function ranges with verified relocated byte equality | No partial-byte credit; unsupported relocations cannot contribute |
| Exact matched functions | Verified function records / all inventory records | Display count and percentage |
| Compared-candidate similarity | Similarity only among compiled comparisons | Secondary diagnostic, clearly labelled; never imply whole-game progress |
| Data matching | Verified reconstructed non-code data / inventoried data | Show unavailable until implemented, rather than a false zero-byte denominator |
| Linking completion | Units/code/data passing the linking completion gates below | Show unavailable while unsupported; matching alone cannot complete a unit |

Keep fuzzy weighting consistent between native reports and the adapter. Define
how ambiguous overlapping records contribute before adding a broader code
denominator; exact byte totals must always avoid double counting. Publish the
algorithm/version and inventory scope so historical scores remain comparable.

Browser cards should show percentages and numerators/denominators. Add per-object
progress bars and optional source-path views while keeping original unity groups
as compilation units. A selected function keeps its fuzzy score, exact status,
unresolved reasons and relocation-target differences. Watcher updates must
preserve selection, filters, active tab and unsaved editor contents.

Acceptance: with two perfect candidates and thousands missing, whole-inventory
progress remains tiny while compared-candidate similarity can be 100%. A 100%
assembly score with unresolved relocations still displays unresolved and adds
zero exact bytes. Empty inventories, missing candidates and overlapping ranges
have deterministic tested denominators. Source/header edits refresh every metric
without a navigation or browser restart. Validate the browser on a real report
and a synthetic partially matching/unresolved report.

## 2. Inventory and compare non-code data

Inventory Mach-O sections, named data symbols, STABS ownership hints, alignment,
zero-fill ranges and aliases. Give data stable IDs and record ambiguous ends,
ownership and overlapping ranges rather than guessing. Separate independent
data from literal pools already contained in function byte comparisons.

Allow each original compilation group to declare reconstructed data candidates
and proven placement maps. Compare full candidate ranges after supported pointer
and section-difference relocations are resolved. Exact initialized bytes and
zero-fill size/alignment are different checks. Keep strings, arrays, jump tables,
Objective-C metadata and vtables visible when no candidate exists. Do not count
copied original sections as reconstructed data.

Extend native reports with data status/counts/bytes, then populate the objdiff
adapter's data fields with real denominators. Keep fuzzy code and any fuzzy data
score separate in native reports, documenting the adapter's combined weighting.
Add a browser data tree/tab and side-by-side hex/typed views with relocation
targets. CI must fail when previously verified data regresses or coverage is lost.

Acceptance fixtures: initialized arrays/strings, BSS, alignment/padding, duplicate
names, pointer arrays, paired relocations, vtables, malformed ranges and unknown
addresses. Wrong pointer targets never match; equivalent resolved pointers can.
Counts and reports agree on Linux and macOS with identical artifacts.

## 3. Prove historical ARMv7 Mach-O linking separately

Pin a Linux-capable cctools/ld64 linker recipe and fingerprints independently of
the compiler. Identify required libraries, frameworks, ABI options and load
commands from the verified executable and SDK. Do not assume the assembler
version currently used for object compilation proves linker equivalence.

First cross-link synthetic C/C++/Objective-C/Objective-C++ programs with SDK 5.1.
Exercise ARM calls, imports, lazy/non-lazy stubs, literal/data references,
constructors, Objective-C metadata and Mach-O section/load-command layout.
Check output architecture, deployment target, symbol bindings and relocation
semantics structurally; report that a structural link test does not prove runtime
behavior. No device or Apple account credentials are needed for these probes.

Then add a deterministic linking manifest recording object order, section order,
alignment, exported symbols, dylibs/frameworks, flags and unresolved imports.
Derive original order/ownership where metadata supports it and mark uncertainty.
Add incremental Ninja linking rules, logs and a `doctor` linker capability check.
Unknown veneers/interworking/import transformations continue to block verified
comparisons until independently tested; linking must not introduce address masking.

Acceptance: pinned native Linux linking probes reproduce under macOS Docker
emulation, SDK inputs remain local/ignored, and deliberate missing imports or
wrong placements fail while retaining reports and diagnostics. Validate a linked
synthetic image independently of the matching core's own relocation formulas.

## 4. Integrate replacement-image and linking progress

A compilation unit becomes complete only when its intended code/data is
reconstructed, coverage is accounted for, its object participates in the intended
link, and configured equality/layout checks pass. A fully matched function in a
partly reconstructed unity unit cannot mark that unit complete.

Add explicit link states (unsupported, blocked, failed, linked, verified), code
and data completion totals, image diagnostics and a browser linking panel.
Distinguish structural linking, verified image equality and runtime validation.
Opaque original chunks, if ever used for diagnostic bring-up, need a separate
category and contribute no recovered-source or verified completion credit.

CI compares base/head with identical IPA, compiler, SDK and linker profiles.
Linker/profile migrations require an explicit baseline rather than silently
erasing earlier verified results. Gate verified code/data regressions, compile
and link failures, and inventory loss; retain reports even on failure. Upload
allowlists continue to exclude original inputs, SDKs and generated game images.

Acceptance: mixed matched/missing units report partial progress correctly; a
deliberate data or link regression fails CI; full image verification accounts for
all declared sections and bindings. Runtime and hosted decomp.dev integration
remain separately scoped work, not implied by a successful object comparison.

## Execution order

1. Browser/report clarity for existing fuzzy and exact measures.
2. Data inventory and exact comparison, with native/adapter/browser/CI coverage.
3. Pinned linker feasibility probes and incremental synthetic link loop.
4. Replacement-image integration and explicit linking completion gates.

These tooling stages are implemented; see [progress and linking](progress.md) for behavior, commands and remaining original-layout uncertainty. The accompanying source work recovers
10–20 additional small functions with the existing historical compiler and byte
verification core; it does not claim a linked or runnable full-game replacement.
