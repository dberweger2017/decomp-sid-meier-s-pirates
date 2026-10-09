# Builds, reports, and CI

`.github/workflows/build.yml` applies Melee’s build-and-report workflow pattern to Pirates’ function comparisons. It does not reuse Melee’s compiler or binary backend.

## Jobs

The synthetic tooling matrix runs on macOS 15 and Ubuntu 24.04 with a pinned Python version, commit-pinned GitHub Actions, and hash-pinned Capstone/Ninja wheels. Tests exercise parser errors, inventory coverage, ARM/Thumb-2, literal pools, relocation pairs, branch targets, byte verification, real compilation, header/source rebuilds, stale-object rejection, the watcher/editor API, and report regressions. A fixed core-report digest is asserted on both hosts, independently of each host’s modern Clang version.

The progress job obtains one archival IPA and checks the fixed SHA-256 before any build. PR base and actual PR head are checked out separately. Push builds compare the previous commit when available. For the initial pre-tooling base, no candidate manifest means zero recovered source progress.

`tools/ci.py build` creates isolated workspaces preserving each revision’s sources and candidates. Both use the head’s comparison engine, verified input identity/inventory lock, and common compiler profile. This avoids attributing differences in the comparison tool or original inputs to source progress. Compiler fingerprints must agree. Once the base has candidates, changing its shared compiler profile fails CI: a compiler migration needs a separately established baseline, rather than silently rebuilding both revisions under new flags and erasing previous matches. Source and per-unit flags may change as part of candidate work; they are recorded in diagnostics and reports.

Ninja compiles incrementally inside each workspace. Source/header depfiles keep unaffected objects intact. Candidate compile rules retain structured results and diagnostics even on failure; the report rule subsequently fails the build if any unit cannot compile. The final comparison fails when a verified function loses its match, a candidate cannot compile, or recovered function/object coverage disappears. Similarity-only regressions are displayed but do not constitute loss of a verified match.

Job summaries show matched bytes/functions, missing source, unresolved comparisons, compile errors, similarity, newly matched IDs, improvements, and regressions. Full-game linking remains distinct at zero completed units. Reports/diagnostics upload on failure. Artifact paths are explicitly limited to JSON, text diagnostics, and summaries; they exclude the IPA, original executable, SDK, source caches, candidate object files, and toolchain binaries.

## Historical compiler and SDK boundary

The historical four-language runtime has passed native Linux source-build validation and ARM/Thumb probes, plus the synthetic historical match/header-regression loop. Units without recovered source retain their inventory and zero matching progress. When either base or head has source units, the progress job builds and validates the pinned compiler recipe using cached sources and build outputs. It does not substitute Clang. Provisioning failures fail CI after preserving reports and diagnostics.

An explicitly configured immutable Linux amd64 image is validated directly; otherwise provisioning generates `build/toolchain/compiler.json` with its local immutable image ID. Both source revisions use that profile through Docker. Generated profiles retain the canonical template hash, so provisioning cannot bypass the compiler-baseline guard. Editor-only settings do not alter that baseline. A local SDK may be supplied through the runner’s `PIRATES_SDK_PATH` repository variable/path, or through `tools/ci.py build --sdk`. Otherwise the runner fetches the third-party mirror pinned in `config/sdk-lock.json`, validates its Git tree, SDK metadata and complete content manifest. Base and head use that identical SDK; changes to the SDK lock after candidates exist require an explicit baseline migration. Ninja checks SDK content even for system headers omitted by `-MMD`. The workflow does not access Apple accounts, commit SDK files, cache them or upload them. Freestanding C/C++ units may explicitly set `sdk_required: false`; other units inherit the profile requirement.

`.github/workflows/compiler-feasibility.yml` runs on compiler-tooling PR changes and can also be dispatched manually. It builds the pinned sources on native Linux, validates C/C++/Objective-C/Objective-C++ ARM/Thumb Mach-O output and repeated object hashes, and exercises a historical synthetic match/header-edit/regression loop. These checks have passed on native Linux and Apple Silicon Docker emulation. Exact equivalence to the shipped compiler and original flags is still unproven. All four SDK-dependent language probes have also passed on native Linux and Apple Silicon emulation. Logs and diagnostics remain available on failure. On success it exports a separate compiler-image artifact containing the compiler sources, licenses and fingerprints; that artifact contains no original game inputs or SDK files.

## Local commands

```sh
# Compare already-produced native reports:
python tools/ci.py compare --base path/to/base.json --head path/to/head.json

# Build two source checkouts using the same original and compiler:
python tools/ci.py build --base /path/to/base --head "$PWD" \
  --ipa /path/to/pirates.ipa --output build/ci-artifacts \
  --profile build/toolchain/compiler.json --validation build/toolchain/validation.json \
  --sdk build/sdk/iPhoneOS5.1.sdk

# Deliberately lose a verified synthetic match (expected exit code 1):
python tools/ci.py demo-regression
```

Outputs are deterministic for identical inputs, compiler artifacts/profiles, flags, and source dependencies. Compiler identity can legitimately differ between host-provided Clang installations; synthetic tests label that compiler and never turn those scores into game progress. There are no timestamps in progress reports. SDK or workspace paths in compiler diagnostics are normalized, while original debug-source paths are preserved as binary provenance.

The current local verification imported the real archive, accounted for all 9,177 function records and 268 groups, started at zero game matches and now verifies 16 source functions at 628 bytes, and detected a deliberate synthetic verified-match regression. Browser automation verified source and included-header saves update the selected comparison without a page reload. Hosted macOS/Linux synthetic jobs and the real-archive progress job also passed; CI uses pinned Python 3.13.16 packages available on macOS ARM64 and Ubuntu x64.

The game-progress job runs `tools/first_candidate_smoke.py` in the staged head workspace. It deliberately loses the real ApiSet match, verifies that the regression checker fails, and restores the match after an included-header rebuild. Those reports and its summary are uploaded separately from base/head progress.

The gameplay stress checkpoint also installs optional hash-pinned
`requirements-emulation.txt`, exercises synthetic ARM call/negative scenarios on
both tooling hosts, and reports the battle-grid and world-map projection cohorts
in the historical progress job. Bounded execution checks use source candidates
and explicitly modeled external calls; they never affect verified matching or
replacement-link credit. Cohort reports, execution outcomes and the two-group
diagnostic link status/diagnostics are retained in `pirates-progress`.
