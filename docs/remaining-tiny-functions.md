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
they do not create substitute globals or synthesize startup registration.

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
python tools/cohort.py report config/cohorts/remaining-tiny.json
python -m unittest discover -v
```

Full-game replacement linking stays separate and receives no credit from these
entry points. The fixed cohort and native regression report account for every
selected record, including any candidate that remains different or unresolved.
