# Progress and linking

The workbench keeps four measures separate: fuzzy code, exact code/functions,
exact data, and linking completion. The first batch now verifies 16 functions
and 628 bytes, with five differing candidates and 9,156 missing function records.
This includes 14 new matches beyond the initial two. See
[easy candidates](easy-candidates.md) for every ID and flag evidence.

## Measures and scope

Code covers the union of recovered STABS function ranges, including literal
pools. It does not include every executable byte or every linker-generated stub.
Exact bytes count whole verified ranges and avoid overlapping-range double
counting. Unknown boundaries, addresses, unsupported relocations and stale
objects cannot earn exact progress, even with 100% assembly similarity.

Overall fuzzy code is instruction/operand sequence similarity weighted by STABS
record size, with missing candidates contributing zero. Overlapping records
retain their individual fuzzy weights; this is stated in the report algorithm
field. Compared-candidate similarity is a secondary, unweighted diagnostic.
This algorithm is ours, not Melee's or stock objdiff's implementation.

Native reports preserve code and data scores separately. The objdiff protobuf
JSON adapter uses strings for uint64 fields and combines code fuzzy similarity
with exact data credit, weighted by their byte denominators. Data sections and
counts have real denominators. Unattributed data has an explicit adapter storage bucket with zero compilation
units, so unit measures reconcile without inventing another original group.
No linking progress follows from function similarity or a compiler probe.

## Data allocations

The original yields 5,056 ranges across 25 non-instruction sections, totaling
3,827,236 bytes, including 3,292,363 zero-fill bytes. 26,606 bytes have no unique
STABS ownership. This includes strings, arrays, exception tables, Objective-C
metadata, vtables, pointers, BSS, padding and unnamed gaps. Literal pools within
function ranges are excluded to avoid double counting. Data starts at zero
source matches for this batch.

Mach-O symbols do not provide C object sizes. The inventory therefore describes
whole allocations through the next named definition or section end, including
padding, and never pretends these are recovered `sizeof` values. Original aliases
share a range. Ambiguous ownership and unnamed boundaries stay explicit;
ambiguous candidate names/aliases remain unresolved. Section storage class,
alignment, allocation size and relocated bytes must agree. Zero-fill compares
allocation size and alignment without constructing huge zero buffers.

A complete source unit can discover owned data by exact symbol name. A partial
unit using `implemented_functions` must explicitly declare data:

```json
"data": {"d-<stable-id>": {"symbol": "_candidate_data_symbol"}}
```

Mappings must belong to their original group, or explicitly identify unattributed
data, and cannot be duplicated across units. Named original data in the same
STABS group resolves before globally duplicated names. Supported pointer and
paired section-difference relocations use original addresses; wrong or unknown
targets do not match. Explicit placements cannot contradict known original symbol
addresses. Imported dyld targets, veneers and interworking still remain unresolved
in object comparisons until their transformations are supported. Instruction relocations inside non-code data are unresolved.

The browser has a data tree, status/source filters, byte/ASCII/little-endian-word
views, relocation targets and source editing. Display is limited to 4,096 bytes;
equality includes the entire allocation. Terminal data inspection uses the same
command as functions: `python tools/dev.py diff d-<id>`.

## Linker setup

Build the compiler and supply the verified local SDK first. Then:

```sh
python tools/linker.py build
python tools/linker.py validate --compiler build/toolchain/compiler.json --sdk build/sdk/iPhoneOS5.1.sdk
python configure.py --linker build/linker/linker.json
ninja
python tools/dev.py doctor
```

The linker is a separate Linux amd64 container with immutable image identity,
locked Ubuntu package versions, content-pinned cctools 845 / ld64 134.9 sources,
recipe provenance and binary fingerprints. Host Clang 3.8 builds ld64; game
candidate objects still come from historical LLVM-GCC. Two host portability
patches bypass Darwin sysctl detection on Linux, retaining default worker count.
They change neither ARM code generation nor linker layout/binding algorithms.

Validation cross-links C/C++/Objective-C/Objective-C++ ARM and Thumb probes twice.
It covers SDK startup, SDK imports, lazy/non-lazy/weak bindings, C++ constructors, Objective-C
class metadata, deployment commands, and rejection of a missing OpenGLES import.
An independent image inspector checks ARMv7, declared entry symbols and ARM thread entry addresses, library ordinals,
binding addresses/addends and expected library targets. Matching-core relocation
formulas do not validate the linker's output. UUID-free probe images repeat
exactly; receiving-host imports rerun the probes and compare hashes. This proves
structural linking, not runtime behavior or exact original linker equivalence.

`python tools/linker.py export` exports the validated open-source linker container
and validation manifest, excluding SDK and original inputs. CI publishes it as
`validated-ld64-linux-amd64`; `python tools/linker.py import <directory>` verifies
and revalidates a downloaded artifact on Linux or macOS/Apple Silicon.

The exported native Linux runtime was imported and revalidated on Apple Silicon:
the linker binary and all eight SDK-dependent ARM/Thumb image hashes and
inspected layouts agreed, including SDK startup, C++ initialization and Objective-C
metadata. Docker/containerd may assign a different immutable image ID on import.
The importer pins that receiving ID only after verifying the binary fingerprint
and repeating the probes; `build/linker/runtime-import.json` records both IDs,
archive hash and validation result. Changed images or linker binaries cannot retain
validation. These probes establish reproducible structural linking, not iOS runtime
behavior or exact equivalence to the game's original linker.

## Manifest and incremental loop

`config/link.json` records object order, libraries, deployment/SDK load-command
versions, entry, layout flags and UUID. It starts disabled because source coverage
is partial. `config/original-link-layout.json` records verified original section,
dylib, binding and identity metadata; it contains no executable bytes. The original thread entry is `start`, not `_main`. SDK `crt1.3.1.o` is structurally
validated as bootstrap input, with exact original startup equivalence unproven.
STABS order is a starting hypothesis, not proof of original linker input ordering. The
original has a 4.2 minimum OS and SDK load-command value zero; embedded build
metadata separately identifies SDK 5.1.

The generated Ninja graph compiles affected units, links/validates when inputs
change, then writes reports, including on compile or link failure. Included
headers, compiler/SDK identities, linker profile and manifest invalidate stale
results. Link inputs are configured source objects, verified SDK startup objects and SDK libraries only;
original objects, opaque section payloads, response-file injection and unresolved
symbol suppression are not accepted. Diagnostics stay under `build/link/` and
failed images are deleted.

A diagnostic manifest can link configured candidates using `scope: "diagnostic"`,
a chosen entry and explicit object order. It produces no linking completion.
Replacement mode requires every original source unit/function and owned data
allocation to verify before linking. Shared or unnamed data is established by
final image equality, avoiding a false requirement to assign linker-generated
sections to a single original object.

States are unsupported, blocked, failed, linked and verified. A structurally linked
image remains separate from a verified image. Linking completion is conservative:
units/code/data earn credit only after full source coverage and a byte-identical
replacement image, not per-function similarity or a partial diagnostic link.
The declared original UUID can be reproduced in its identity field without
changing addresses; this patch is recorded, and full-image SHA-256 equality still
checks every resulting byte. The whole game remains blocked with zero complete
units. Original flags/layout and linker equivalence remain investigations.

## CI and browser

Both supported hosts run deterministic synthetic parsing, relocation, data,
report, watcher and incremental-link lifecycle tests. Actual historical compiler
and SDK linker tests run in the pinned Linux containers. Base and head use one
engine and identical original inputs, compiler, SDK and linker profiles. Existing
compiler/SDK/linker baselines cannot be silently reset. Verified function/data
regressions, disappearing inventory, compilation failures, linking failures and
loss of a verified image fail CI; reports and diagnostics remain available.
Uploads exclude IPA, executable, SDK and generated game images.

The browser shows overall and compared-only fuzzy scores separately, exact
percentages/counts, per-original-object bars, data status and linker capability,
coverage gates and diagnostics. Watcher refreshes preserve selection, filters,
active tab and unsaved source contents. Native and objdiff reports remain
available in `build/report.json` and `build/objdiff-report.json`.
