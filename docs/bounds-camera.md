# Bounds transforms and camera visibility

This batch recovers the ordinary-source call chain from saved AABB extents to
camera visibility: the transform, matrix/vector product, vector arithmetic,
planes, frustum refresh and box visibility. It adds 18 previously missing source
bodies covering 6,872 recorded bytes. They compile and compare with resolved
references; only the 40-byte default plane constructor is newly exact.

The full historical build has **1,615 exact functions / 18,300 bytes**, 199
differing candidates, 7,352 missing and 11 unresolved comparisons. Against the
bounds-foundation baseline, no exact matches, data matches or assembly
similarities regress; compilation errors remain zero. The 22-entry cohort
includes four previous vector matches and has five exact entries / 100 bytes,
with 40.8181% byte-weighted assembly similarity. Fuzzy similarity is a comparison
heuristic, not behavioral or completion credit.

| Recovered source | Original bytes | Assembly similarity |
|---|---:|---:|
| `ISEAABBBox::transform` | 1,404 | 39.1185% |
| `ISECamera::IsVisible(ISEAABBBox&)` | 520 | 55.5985% |
| `ISECamera::updateFrustumPlane` | 3,080 | 29.9328% |
| `NEON_Matrix4Vector4Mul` | 32 | 2.7027% |
| Eight vector math bodies | 500 | 10.5263–96.2963% |
| Six plane bodies | 1,336 | 25–100% |

The scalar historical compiler emits a much larger matrix/vector helper than
the original eight NEON instructions. The source uses ordinary GNU vector
expressions; it contains no instruction payload or inline assembly. Recovering
the remaining compiler/intrinsic source shape is separate matching work.

## Layout and behavior evidence

Original loads and stores establish the AABB's current upper/lower bounds at
`+0x00`/`+0x0c`, saved upper/lower bounds at `+0x18`/`+0x24`, and empty byte at
`+0x30`. Transform returns immediately for an empty box. Otherwise it resets
the current bounds and merges the eight transformed saved corners in observed
order: LLL, LLU, LUU, LUL, UUL, UUU, ULU, ULL. Every input has w = 1; output w
is discarded, without perspective division. Repeated transforms start from
saved extents again. The matrix helper reads four contiguous columns and all
input coordinates before storing four output coordinates, including aliases.

The vector value occupies three floats at offsets 0, 4 and 8. Its assignment
preserves the existing word-copy match, including self-copy and NaN bits.
Normalization computes a float square root and substitutes length 1 for a
zero vector. Absolute dot products round each multiplication to float, promote
each magnitude to double, sum in double, then return float, following the
original conversion instructions.

The plane is a normal vector member at `+0x00` and distance at `+0x0c`, with
16-byte stride. Composition matters: its original constructors call the
complete vector constructor, rather than a base constructor. Classification
compares signed center distance against the absolute-dot half-size radius:
strictly negative returns 1, strictly positive returns 0, and intersections
(including touching and unordered comparisons) return 2. The center temporary
is destroyed before the radius call. Triangle construction uses
`(c-a).CrossProduct(b-a)`, normalizes it, and stores minus the first vertex's
dot product. That winding follows the original instructions.

| Observed camera offset | Recovered interpretation |
|---|---|
| `0x120` | Existing perspective flag |
| `0x124`, `0x130`, `0x13c` | Position, target and up vectors |
| `0x148` | Field of view in radians; `tan` receives half the angle as double |
| `0x150`, `0x154` | Near and far distances |
| `0x160` | Six planes, each 16 bytes |
| `0x1c0` | Dirty byte; cleared after refresh |

Refresh follows the original forward/right/vertical cross products, eight
corners and six triangle orders. It uses the original fixed 4:3 aspect literal
`0x3faaaaab`; these observed methods do not consult the perspective flag or
select an orthographic path. Visibility refreshes if dirty, then rejects the
box when any plane classifies its center/half-size as negative. Unknown camera
prefix fields, hierarchy, virtual slots and other methods remain unrecovered.
The raw-memory execution fixtures do not instantiate a complete camera class.

## Exception references and investigation flags

Camera and plane calls have compiler-generated SjLj landing-pad tables.
Comparison placements use evidence for the entire emitted table sections,
rather than ordinal local labels such as `GCC_except_table2` (whose numbering
changes with source recovery).

| Original group | Table section address | Size | SHA-256 of emitted and original bytes |
|---|---|---:|---|
| Camera | `0x3fb3a8` | 232 | `d1585d3679c1b0daad3087e1d87c686e19d3e6c5587a162be2fcd0132dd09eec` |
| Plane | `0x3fbd70` | 64 | `ff51183076a6e8bef6d058bd6f6c2fadf0dd102d5bb3081cf2b9600ca879982c` |

The original camera refresh loads displacement `0x6bc60` from `0x390314`
and adds PC base `0x38f748`, identifying `0x3fb3a8`. Visibility loads
`0x6b0dc` from `0x39051c` and adds `0x39039c`, identifying its trailing
24-byte table at `0x3fb478`. Plane float-coordinate distance loads `0x48d24`
from `0x3b312c` and adds `0x3b304c`, identifying `0x3fbd70`. Triangle creation
loads `0x48aac` from `0x3b34a4` and adds `0x3b32e4`, identifying its 32-byte
table at `0x3fbd90`; classification occupies the intervening 16 bytes.

Both original objects' personality references identify the nonlazy slot at
`0x4566c0`. The original `__DATA,__nl_symbol_ptr` starts at `0x455014`, with
indirect-table base index 366; this slot selects index `0x319`, symbol index
`0xe2fe`, named `___gxx_personality_sj0`. The candidate nonlazy sections contain
only that pointer. These addresses resolve comparison references; the actual
subset linker chooses its own section layout and SDK bindings.

Softfp preserves ARM integer-register argument passing while allowing the
observed hardware floating-point instructions. Vector `-fno-math-errno`
lowers the observed square root without introducing an SDK `sqrtf` call.
Plane `-fno-toplevel-reorder` preserves the verified exception-table order;
disabling post-RA scheduling restores the default constructor's frame-pointer
order. These remain investigation settings, not proof of all original flags.
All previously exact entries in the affected groups remain exact. The emitted
tables are compared during the optional probe and earn no new data-record
credit. No original bytes are used as source, objects or link inputs.

## Acceptance evidence and reproduction

All 142 repository tests pass, including the expanded host AABB check for
reflection, translation, non-unit w, repeated transforms and empty no-op.
The optional ARM probe executes the original image and the actual linked
candidate image, with no instruction rewriting or fallback original callees.
**127/127 fixtures pass**, with maximum original/candidate float difference
**0.0** in these fixtures. Cases include matrix aliases, 24 seeded matrix
inputs, transformed corners, both windings, zero-vector normalization,
signed half sizes, NaN classification, touching boundaries, rejection by each
cached plane, shifted/tilted cameras, zero forward/parallel up, near/far tests
and repeated dirty-to-clean refresh.

The five-group subset links with the pinned historical compiler, ld64 and
SDK 5.1. Its retained geometry/math callees all come from recovered source;
`tan`, SjLj and exception services have explicit SDK library binding checks.
Unreachable incomplete camera construction is dead-stripped. The resulting
image hash is `63bc114749a77da963bf58b41430dc3cb53ce2087882254315dad4ff1c005aa6`.
The probe models only normal-return `tan` and SjLj frame registration lifetimes;
exception unwinding, denormal/nonfinite math beyond the explicit NaN case,
process startup, iOS runtime and whole-game equivalence remain unverified.
The subset earns zero full-game replacement/link completion credit.

```sh
ninja
python -m unittest discover -s tests
python tools/cohort.py report config/cohorts/bounds-camera.json \
  --output build/bounds-camera-cohort.json
python tools/recovery_link.py config/diagnostic-links/bounds-camera.json
# Optional local behavior acceptance, with requirements-emulation.txt installed:
python tools/bounds_camera_probe.py
```

Reports remain under ignored `build/`: `bounds-camera-baseline.json`,
`bounds-camera-regression.json`, `bounds-camera-cohort.json`,
`bounds-camera-execution.json`, and `recovery-link/`. No mandatory CI jobs were
added. The next roadmap batch is controller/keyframe interpolation and
versioned sequence loading, with owned data, bounds, version and lifetime
fixtures before expansion into its callers.
