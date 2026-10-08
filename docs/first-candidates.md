# First recovered source candidates

This milestone adds two small PowerVR library functions from the original executable. They are compiled C++ source, not assembly or original-byte fallbacks. They verify at 2 functions / 84 bytes; the other 9,175 function records remain missing. Original object and unity groups are preserved.

## PVRShellInit::ApiSet

- Function ID: `f-62cd60b098a1a0381e31`.
- Original group: `PVRShellAPI.o`, source hint `Shell/API/EAGL/PVRShellAPI.cpp`.
- Original address: `0x2fc4`; ARM; 8 bytes.
- Recovered behavior: return false without reading arguments or object fields.

The Darwin mangled name establishes the class, method and argument types. The public [PowerVR declaration preserved in SwiftShader](https://swiftshader.googlesource.com/SwiftShader/+/f97ba4d02cc7383fcf3ee14dcb25af89b3b82355/third_party/PowerVR_SDK/Shell/PVRShellImpl.h) corroborates the boolean return type, which an ordinary C++ mangled name does not encode. The EAGL behavior comes from this executable's complete function instructions, not the other platform's implementation. The source header contains only the minimal ABI declarations: enumeration values and class layout remain unrecovered. No object is constructed and no layout-dependent operation is performed.

## PVRTMatrixIdentityF

- Function ID: `f-da79232dc420a34aba1b`.
- Original group: `libOGLES2Tools.a(PVRTMatrixF.o)`.
- Original address: `0x37cf18`; ARM; 76 bytes.
- Recovered behavior: initialize sixteen contiguous floats to a 4x4 identity matrix.

The symbol establishes a reference to `PVRTMATRIXf`. Original stores establish the 64-byte float storage and assignments: diagonal offsets 0, 20, 40 and 60 receive 1.0f, and the remaining elements receive zero. The candidate expresses these assignments in C++; it contains no embedded original instructions or byte arrays. Other matrix methods are not supplied as stubs.

## Flag evidence

Both units currently use the investigation flags `-O2 -marm` plus the common compiler profile. These are successful flags, not recovered original flags. Reproduce the experiments with:

```sh
python tools/flags.py f-62cd60b098a1a0381e31
python tools/flags.py f-da79232dc420a34aba1b
```

The command compiles isolated variants under `build/flag-experiments/` and records source/header/input/SDK/compiler fingerprints, object hashes, byte verification, similarity and diagnostics. It does not change the active manifest or count experimental objects as progress.

For both candidates, ARM `-O1`, `-O2`, `-O3` and `-Os` produce verified equality; `-O0` differs. Thumb variants cannot verify against these ARM originals. Thus these functions do not distinguish the successful optimization levels or prove compiler equivalence throughout the binary. Flags for other original units must be investigated independently.

Both recovered candidates contain no relocations. No new relocation form was needed for these matches. Imported Objective-C runtime targets and other previously unsupported cases remain unresolved until their addresses and linker transformations can be established; SDK compilation probes do not count as recovered source.

## Regression checks

`python tools/first_candidate_smoke.py` stages an ignored temporary workspace, verifies the first source match, changes its return value, checks that the verified match regresses, then restores source and changes an included header. It verifies stale-header rejection and recovery after incremental rebuilding. The original source tree is unchanged. CI runs this same real-candidate check and includes its summary and native reports.
