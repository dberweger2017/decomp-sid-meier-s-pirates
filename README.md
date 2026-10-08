<img src="assets/icon.svg" width="64" height="64" alt="">

# Sid Meier’s Pirates! function workbench

Choose a function, edit its candidate source, rebuild with Ninja, and inspect ARMv7 assembly and verified byte equality. The local browser watches sources and included headers; CI compares base and head progress using identical original inputs and compiler profiles.

**Current status:** the verified archive yields **9,177 function records, 268 original object groups, and 4,080,584 function bytes**. Recovered PowerVR C++ source verifies at **16 matched functions and 628 matched bytes**; five candidates differ and 9,156 remain missing. [Candidate evidence and flag experiments](docs/easy-candidates.md) explain the scope. Unity compilation units remain intact. No replacement game is linked.

**Historical compiler:** Apple’s open-source LLVM-GCC 2336.9 now builds and passes C/C++ ARM and Thumb Mach-O probes on native Linux and through Docker emulation on Apple Silicon. The pinned container also passes the synthetic Ninja match/header-edit/regression loop. Exact equivalence to the shipped Xcode compiler and original flags remains unproven. C, C++, Objective-C and Objective-C++ object probes pass with SDK 5.1/build 9B176. The separately pinned ld64 linker passes eight SDK-dependent ARM/Thumb structural probes on native Linux and Apple Silicon emulation; full-game linking remains blocked by incomplete source/data/layout. [Compiler evidence](docs/toolchain.md) records the results. Modern Clang supplies synthetic fixtures and editor indexing only.

## Setup

Python 3.9 or newer and a C++ editor are sufficient to inspect the original inventory. Matching the included candidates also requires Docker and the locally fetched SDK. Use the virtual environment so the pinned Ninja and Capstone versions are on PATH. The watcher and CI runner invoke the installed pinned Ninja directly, even when another system Ninja is present:

```sh
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --require-hashes -r requirements.txt
python tools/toolchain.py build
python tools/sdk.py fetch
python configure.py --ipa Sid_Meier_s_Pirates__1.1.2_ios_4.2.ipa \
  --profile build/toolchain/compiler.json --sdk build/sdk/iPhoneOS5.1.sdk
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

Build and select the historical compiler before adding candidates (Docker must be running):

```sh
python tools/toolchain.py build
python configure.py --profile build/toolchain/compiler.json
ninja
python tools/dev.py doctor
```

Alternatively, load the validated compiler artifact from the compiler CI workflow as described in [toolchain setup](docs/toolchain.md). The generated profile pins the local image by immutable ID. `doctor` checks that profile and its validation fingerprint. A local iPhoneOS 5.1 SDK is still needed for SDK-dependent candidates; inventory browsing and freestanding probes work without it.

The importer checks the recorded IPA and executable SHA-256, bundle/version/build metadata, ARMv7 architecture, encryption status, and inventory coverage. It extracts only the executable into `build/inputs/`. The provenance is explicit: this is an archival upload, not an authenticated Apple CDN original. See [config/identity.json](config/identity.json) and the existing research records.

## Candidate configuration

[config/candidates.json](config/candidates.json) contains five partial original compilation groups with 21 source candidates. Map a source translation unit to an original object group. Use group IDs from `build/inventory.json`; compile unity sources as a unit rather than splitting them into artificial objects.

```json
{
  "version": 1,
  "units": [
    {
      "group_id": "o-100eccb225782070945f",
      "source": "src/powervr/PVRShellAPI.cpp",
      "flags": ["-O2", "-marm"],
      "implemented_functions": ["f-62cd60b098a1a0381e31"],
      "functions": {}
    }
  ]
}
```

This example selects the initial source candidate; its flags are successful investigation settings, not evidence of original flags. Flags in [config/compiler.json](config/compiler.json) are an investigation profile; record optimization flags, defines, include paths, and mode flags explicitly per unit. `compile_commands.json` uses the explicitly labelled Clang syntax/indexing command for local editors. Ninja compiles matching candidates with the selected historical container. The editor compiler never supplies matching objects or compiler validation.

A function can map to a different candidate symbol with `"functions": {"<function-id>": {"symbol": "<candidate-symbol>"}}`. Use `candidate_address` only to select an otherwise ambiguous candidate symbol. Ambiguous original boundaries still prevent verification. Explicit `placements.symbols` and `placements.sections` can supply proven original addresses for otherwise unresolved references; document that evidence with the unit. For a partly recovered unit, list its recovered IDs in `implemented_functions`; other functions stay missing even if that unit stops compiling. Without this optional list, candidate symbols are discovered automatically. Removing a verified function from the list is still a CI regression.

For a freestanding C/C++ unit that needs no SDK headers, explicitly set `"sdk_required": false` on that unit. Other units inherit the profile’s SDK requirement. All four frontends have validated ARM/Thumb probes.

Fetch the pinned third-party SDK with `python tools/sdk.py fetch`, or supply SDK files locally with `python configure.py --sdk /path/to/iPhoneOS5.1.sdk` after importing inputs. [SDK setup](docs/sdk.md) explains obtaining it from old Xcode; no phone is needed. SDKs, IPA files, executable bytes, toolchain caches, and generated outputs are ignored by Git. Do not commit or upload them.

## Reports and CI

- `build/report.json`: native statuses, verified bytes/functions, missing candidates, unresolved comparisons, separate assembly similarity, compiler fingerprints, flags, and unit diagnostics.
- `build/objdiff-report.json`: an adapter to the pinned objdiff v2 protobuf JSON schema. It exports progress; stock objdiff does not perform these ARMv7 Mach-O comparisons.
- `build/units/`: candidate objects, dependency records, compilation results, and diagnostics.

[Matching semantics](docs/matching.md) describe supported relocations and conservative unresolved cases. Non-code data matching and guarded image linking have separate measures. Current data matching and full-game linking remain at zero; replacement linking is supported but blocked by missing coverage and original layout. [Progress and linking setup](docs/progress.md) explains the pinned linker and manifest.

[CI configuration](docs/ci.md) runs synthetic tests on macOS and Linux, checks a fixed report digest on both hosts, and builds PR base/head with a shared verified IPA and profile. Previously verified matches, compilation health, and inventory coverage are regression gates. Native reports, adapters, summaries, and diagnostics are retained on failure; original inputs and SDKs are excluded from uploads. The compiler workflow runs on relevant PR changes and manual dispatch, caches the pinned source build, validates the historical loop, and exports a compiler image. Candidate-progress CI provisions the same compiler when source units are present and retains reports if provisioning fails.

To inspect a deliberate synthetic regression locally:

```sh
python tools/ci.py demo-regression
```

It intentionally exits with status 1 and writes the before/after reports and summary under `build/ci-demo/`. Hosted decomp.dev registration is outside this milestone.

See [progress, data and linking](docs/progress.md) for separate fuzzy/exact/data measures, the pinned linker, CI gates and [the first 16 exact source matches](docs/easy-candidates.md).

The [public browser](https://pirates.davideb.ch) shows the latest successful `main` build. See [hosting and automatic deployment](docs/hosting.md) for the read-only export, CI gates, Caddy setup, rollback and browser checks.
