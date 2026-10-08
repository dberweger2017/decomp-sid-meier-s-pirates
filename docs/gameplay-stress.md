# Larger gameplay routine experiment

Baseline: merged PR #3, 411 exact functions / 4,864 code bytes, one exact
four-byte data allocation, and zero complete replacement-link units.

This experiment targets BattleGrid's land-battle square visibility/color update
and its property lookup dependency. The original square update is 328 bytes:
conditional bit fields, a material-property call, per-index vertex alpha updates,
and a geometry dirty flag. The property lookup traverses a linked list and calls
an observed virtual type getter. Nearby 476-byte texture-coordinate generation
is a further stress target with allocation, floating-point calculations and
nested loops. All belong to their original unity compilation units.

Record target IDs, exact versus differing outcomes, incremental compile times,
source/profile trials and a source-only dependency link. Compare all previous
verified matches against the saved baseline. SDK/compiler/linker and original
inputs stay ignored. A subset link earns no full-game completion. In particular,
recovering battle-grid display routines does not establish battle simulation,
complete class layouts or iOS runtime behavior.

Work in progress. Evidence and remaining limitations will replace this paragraph
before the PR is ready for review.
