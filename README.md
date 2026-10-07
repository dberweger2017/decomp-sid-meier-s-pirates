<img src="assets/icon.svg" width="64" height="64" alt="">

# Sid Meier’s Pirates! function workbench

Choose a function, edit its candidate source, rebuild with Ninja, and inspect ARMv7 assembly and verified byte equality. The local browser watches sources and included headers; CI compares base and head progress using identical original inputs and compiler profiles.

**Current status:** the verified archive yields **9,177 function records, 268 original object groups, and 4,080,584 function bytes**. There are no game candidates: **0 matched functions and 0 matched bytes**. Unity compilation units remain intact. This milestone does not decompile game functions or link a replacement game.

**Historical compiler limitation:** the LLVM core built in a pinned Linux amd64 research environment, but the LLVM-GCC frontend and ARMv7 Mach-O cross-build have **not been validated**. Docker terminated during the next build attempt. The research image is not a working matching toolchain. The source tag and backend are hypotheses, and the original flags remain unknown. [Compiler evidence and next steps](docs/toolchain.md) describe the concrete results. Modern Clang is used only for synthetic tests; real-game candidates require historical validation.

## Setup

Python 3.9 or newer and a C++ editor are sufficient to inspect the original inventory. Use the virtual environment so the pinned Ninja and Capstone versions are on PATH. The watcher and CI runner invoke the installed pinned Ninja directly, even when another system Ninja is present:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --require-hashes -r requirements.txt
python configure.py --ipa Sid_Meier_s_Pirates__1.1.2_ios_4.2.ipa
ninja
python tools/dev.py serve
```

The server opens <http://127.0.0.1:8765>. Use `--no-open` to leave browser navigation to your editor, or `--port 8766` to choose another port. The server binds to loopback. Browser saves and external editor saves trigger the same incremental build and refresh the current function without navigating away.

```sh
python tools/dev.py diff f-62cd60b098a1a0381e31
python tools/dev.py diff f-62cd60b098a1a0381e31 --json
python tools/dev.py doctor
python -m unittest discover -v
```

`doctor` currently exits unsuccessfully because the historical compiler and local iOS 5.1 SDK are unavailable. Inventory browsing and the synthetic development loop work independently.

The importer checks the recorded IPA and executable SHA-256, bundle/version/build metadata, ARMv7 architecture, encryption status, and inventory coverage. It extracts only the executable into `build/inputs/`. The provenance is explicit: this is an archival upload, not an authenticated Apple CDN original. See [config/identity.json](config/identity.json) and the existing research records.

## Candidate configuration

[config/candidates.json](config/candidates.json) deliberately starts empty. Map a source translation unit to an original object group. Use group IDs from `build/inventory.json`; compile unity sources as a unit rather than splitting them into artificial objects.

```json
{
  "version": 1,
  "units": [
    {
      "group_id": "o-100eccb225782070945f",
      "source": "src/PVRShellAPI.cpp",
      "flags": ["-O2", "-marm"],
      "functions": {}
    }
  ]
}
```

This is a configuration example, not recovered game source or evidence of original flags. A source file must be supplied before building. Flags in [config/compiler.json](config/compiler.json) are an investigation profile; record optimization flags, defines, include paths, and mode flags explicitly per unit. `compile_commands.json` is generated for editor integration. If the compiler is unavailable, it records the intended LLVM-GCC command; it does not select Clang as a matching compiler.

A function can map to a different candidate symbol with `"functions": {"<function-id>": {"symbol": "<candidate-symbol>"}}`. Use `candidate_address` only to select an otherwise ambiguous candidate symbol. Ambiguous original boundaries still prevent verification. Explicit `placements.symbols` and `placements.sections` can supply proven original addresses for otherwise unresolved references; document that evidence with the unit. Unimplemented functions remain missing even when another function in the same unit has source.

Supply SDK files locally with `python configure.py --sdk /path/to/iPhoneOS5.1.sdk` after importing inputs. SDKs, IPA files, executable bytes, toolchain caches, and generated outputs are ignored by Git. Do not commit or upload them.

## Reports and CI

- `build/report.json`: native statuses, verified bytes/functions, missing candidates, unresolved comparisons, separate assembly similarity, compiler fingerprints, flags, and unit diagnostics.
- `build/objdiff-report.json`: an adapter to the pinned objdiff v2 protobuf JSON schema. It exports progress; stock objdiff does not perform these ARMv7 Mach-O comparisons.
- `build/units/`: candidate objects, dependency records, compilation results, and diagnostics.

[Matching semantics](docs/matching.md) describe supported relocations and conservative unresolved cases. Full-game linking measures remain zero and are explicitly marked unsupported.

[CI configuration](docs/ci.md) runs synthetic tests on macOS and Linux, checks a fixed report digest on both hosts, and builds PR base/head with a shared verified IPA and profile. Previously verified matches, compilation health, and inventory coverage are regression gates. Native reports, adapters, summaries, and diagnostics are retained on failure; original inputs and SDKs are excluded from uploads. A separate manual workflow reproduces the compiler feasibility experiment.

To inspect a deliberate synthetic regression locally:

```sh
python tools/ci.py demo-regression
```

It intentionally exits with status 1 and writes the before/after reports and summary under `build/ci-demo/`. Hosted decomp.dev registration is outside this milestone.
