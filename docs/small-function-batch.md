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
