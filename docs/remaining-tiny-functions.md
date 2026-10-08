# Remaining tiny source entries

This batch freezes the 263 simple-shaped records still missing at the PR7
checkpoint: 125 direct calls, 118 empty/identity bodies, 19 constant results and
one field getter. The cohort includes one complete-destructor entry independently
recovered on PR6; it is new relative to this branch's baseline, not new research.

Source compilation always uses the validated historical LLVM-GCC container.
Verified progress requires full relocation-resolved byte equality, including the
named branch target. It does not establish the original declarations, complete
class layouts, runtime metadata, global lifetime registration or compiler flags.
No original-byte or instruction implementations are used.

`config/abi-forwarders.json` reviews entry calls. Constructors with nontrivial
value arguments forward an existing indirect argument object in r1, as observed
in the original C2 bodies. They do not construct a new copy. Registration hooks
and destruction callbacks retain local linkage and remain explicit entry bodies;
they do not create substitute game globals or synthesize startup registration.
Private bodies have unique, unrooted source-emission address references. These
scaffolding pointers receive no original data credit and can be dead-stripped
alongside their callbacks. The historical Darwin `used` attribute would instead
set `N_NO_DEAD_STRIP`, pulling missing callees into diagnostic subsets.

`config/tiny-entries.json` reviews ABI leaf bodies. Empty bodies ignore their
arguments, constant bodies return the observed result, and identity bodies return
the incoming pointer. Opaque ignored parameters and unencoded return types remain
hypotheses. The field getter uses the observed render-unit subobject view:
vertex count at complete-object offset 252, minus the thunk's 180-byte adjustment,
is offset 72 from the incoming pointer. Full inheritance stays unrecovered.

The GLES2, Core Foundation time and printf wrappers include local SDK headers,
so imported call signatures come from those headers. The one-byte alpha-test
visitor is a real by-value C++ aggregate; traversal remains external. Objective-C
leaf definitions use argument/result encodings independently read from the
original runtime method lists. Their partial interfaces are not complete class
hierarchies or replacement runtime metadata and must not be instantiated.

Useful checks after configuring the original inputs:

```sh
python tools/abi_forwarders.py config/abi-forwarders.json --check-source
python tools/tiny_entries.py config/tiny-entries.json --check-source
ninja
python tools/cohort.py report config/cohorts/remaining-tiny.json --output build/remaining-tiny-report.json
python -m unittest discover -v
```

Full-game replacement linking stays separate and receives no credit from these
entry points. The fixed cohort and native regression report account for every
selected record, including any candidate that remains different or unresolved.

## Verified checkpoint

All 263 entries compile and verify exactly: 2,632 additional original code bytes
in 131 original groups, committed one group at a time. Branch totals are 1,574
exact functions / 17,524 bytes, 40 differing candidates, 7,563 missing candidates,
and zero compile errors or unresolved comparisons. All 1,311 earlier function
matches and the exact four-byte `npos` data allocation remain verified.

The complete 1,430-record simple-shaped catalogue is now exact. The broader
1,682-record inventory of functions at most 16 bytes still has 143 missing and
26 differing records outside that catalogue. Source count is not gameplay effort.

117 local tests pass. Synthetic tests reject changed constant/getter instructions,
incorrect named imports and malformed review entries, execute leaf return values,
link duplicate local callback names, compile natural Objective-C methods, and
prove that `--require-exact` fails incomplete cohorts after exporting reports.
Historical compilation, source review validation, doctor and the three existing
diagnostic link graphs pass. All three diagnostic image hashes remain unchanged;
no new live linked dependencies or replacement completion is implied.

CI requires this fixed shortlist to remain exact. It continues to build PR base
and head with identical input/compiler profiles, checks older match regressions,
and exports the native report, objdiff adapter, cohort details and diagnostics.
IPA/executable inputs, SDK headers, toolchain caches and generated outputs stay
ignored. Hosted checks are distinct from these local results.
