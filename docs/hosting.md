# Public function browser

The read-only browser lives at **https://pirates.davideb.ch**. It shows merged
`main` progress. Unmerged recovery PRs do not contribute to the live counters.
Exact code, fuzzy code, exact data and complete replacement linking retain the
local workbench's distinct meanings. Hosting does not establish a runnable game
or exact historical compiler equivalence.

## Updates

`main` push → Linux/macOS tooling tests and historical base/head build → exported
snapshot → restricted SSH upload → independent provenance/content checks →
atomic release switch and HTTP health check.

The deployment job is part of `.github/workflows/build.yml`, requires both build
jobs, and runs only for a `push` to `main` in this repository. PR snapshots are
uploaded as `pirates-site` artifacts for inspection. Failed compilation or a CI
regression leaves the previous site online. Superseded workflow runs may be
cancelled; the server rejects any SHA that no longer equals current `main`.

Open pages check `deployment.json` every 30 seconds. A changed release reloads
the page with its selected function/data ID preserved in the URL fragment.
The header links to the deployed commit and shows the publication time. Details
are fetched from the selected immutable release, so an upload cannot mix the
original tree with another release's assembly or source.

No public webhook receiver is needed: successful CI invokes the existing SSH
service with a dedicated key. This avoids another service and signing secret.
The server independently checks the GitHub run's repository, workflow path,
push event, branch, head SHA and successful required jobs before promotion.
These public API checks use no GitHub credential; if GitHub is unavailable or
rate-limits them, deployment fails and the previous site stays available.

## What is published

The exporter recomputes comparisons in one pass and requires agreement with the
completed build's report. It writes a deterministic native report, objdiff
adapter, per-record assembly/data display, compiler diagnostics, tracked local
candidate sources/headers, static browser assets, and a SHA-256 content manifest.
CI stages a tracked-file allowlist from the original checkout. SDK headers,
local untracked files, dependency inventories and absolute workspace paths are
excluded or normalized.

The site contains no IPA, original executable file, SDK, candidate objects,
linker image, toolchain, credentials, local editor endpoints or build watcher.
Original function assembly/hex and bounded non-code data displays remain visible
as reference material, with missing source contributing zero matching progress.
The source tab is read-only; recovery still happens in the local checkout.

To export a configured, built workspace:

```sh
python tools/export_site.py --workspace . --output build/site \
  --commit <full-Git-SHA> --updated-at <UTC-commit-time>
```

`--updated-at` uses `YYYY-MM-DDTHH:MM:SSZ`. Supply the same commit time and optional
`--run-url` to reproduce the same snapshot. Use a new output directory each time.
The server's `published_at` is operational metadata outside the snapshot.

## Server layout

The `server` SSH alias reaches the existing Debian Netcup host. Pirates uses the
existing Docker Compose/Caddy architecture and external `proxy-nw` network:

- `/opt/code/pirates/compose.yaml` and `Caddyfile`: root-owned static service
  configuration, with the Caddy image pinned by immutable registry digest.
- `/opt/code/pirates/releases/<commit>-<manifest-hash>/`: immutable snapshots.
- `/opt/code/pirates/current`: atomic symlink to the serving release.
- `/opt/code/pirates/state/deployment.json`: atomic public release pointer.
- `/usr/local/lib/pirates-deploy.py`: root-owned copy of `deploy/receive.py`.
- `pirates-deploy`: dedicated system account, without Docker/sudo membership.
  Its root-owned `authorized_keys` entry uses `restrict` and a forced receiver
  command; shell commands, PTYs, forwarding and file-transfer commands are denied.

The gateway stanza in `deploy/gateway.caddy` proxies to `pirates-site:80`. The
site's health port `8766` is published only on loopback. DNS points the Pirates
subdomain to this host; the shared gateway manages HTTPS. Caddy configuration
was validated before reload, with the pre-change gateway file backed up under
`/opt/code/gateway/backups/`. Existing gateway bind-mount inodes must be preserved
when editing its configuration.

The receiver never executes uploaded code. It rejects links, unexpected files,
duplicate tar entries, traversal, excessive sizes and manifest hash mismatches.
Production snapshots must identify the pinned original executable, all 9,177
function records and 268 groups, and a validated historical compiler. A failed
build, disappearing inventory, or regression of an existing verified code/data
match cannot replace the live release. A failed serving health check restores
the previous pointer/symlink. Keep the last five releases and any younger than
24 hours, giving open pages time to finish reading their immutable snapshot.

## GitHub configuration and key rotation

The `pirates-production` environment allows deployments only from `main`.
It contains secret `PIRATES_DEPLOY_KEY` and variables `PIRATES_DEPLOY_HOST`,
`PIRATES_DEPLOY_USER`, and `PIRATES_DEPLOY_KNOWN_HOSTS`. The host key was obtained
through the already trusted `server` connection; CI uses strict host checking.
The deployment key is separate from the operator's SSH keys.

To rotate it, generate a new Ed25519 deployment key into an ignored/private
directory, install its public key with the same forced-command restriction,
and set the GitHub environment secret through stdin:

```sh
gh secret set PIRATES_DEPLOY_KEY --env pirates-production < /private/path/new-key
```

Test the new key, then remove the superseded public-key entry and local private
copy. Operator access through `ssh server` remains independent. Receiver/config
updates are reviewed Git changes installed through the operator connection;
ordinary CI uploads can replace website content only.

## Verification and operations

```sh
python -m unittest discover -v
curl --fail https://pirates.davideb.ch/deployment.json
ssh server 'cd /opt/code/pirates && docker compose ps'
```

The optional browser smoke test uses Playwright 1.64.0 in an ignored directory:

```sh
npm install --prefix build/browser --save-exact playwright@1.64.0
build/browser/node_modules/.bin/playwright install chromium
NODE_PATH=build/browser/node_modules node tests/browser/hosted.cjs
```

Set `PIRATES_SITE_URL` for another deployment, `PIRATES_BROWSER_EXECUTABLE` to use
an existing isolated Chromium binary, and `PIRATES_BROWSER_SCREENSHOT` to save a
screenshot. `PIRATES_WAIT_FOR_REFRESH=1` additionally waits up to three minutes
for a real release switch and checks selection preservation. It does not edit
sources or publish anything. The smoke test checks the first verified API
candidate and the recovered `CPVRTString::npos` data allocation.

## Progress treemap

Each overview block represents an original STABS compilation object, including
unity units. Its area follows the union of recovered function byte ranges.
Select a block to see individual functions, sized by their original record
length (including literal pools), then select a function to open its comparison.
Archive filtering groups original `lib*.a(object)` records without inventing new
compilation boundaries. Unattributed data has a separate group in data mode.

The grouping follows the object/function overview used by
[decomp.dev](https://github.com/encounter/decomp.dev/blob/main/crates/web/src/handlers/report.rs).
The binary layout and canvas renderer here are independent implementations.
Block colours interpolate continuously from grey at 0% to green at 100%:

- **Exact code:** verified byte equality; group colour is byte-weighted progress.
- **Fuzzy code:** byte-weighted assembly similarity, including unresolved
  comparisons. A green fuzzy block does not establish an exact match.
- **Exact data:** verified whole-allocation equality, including padding and
  zero-fill; area follows original data bytes.
- **Replacement linking:** green only for verified complete replacement units.
  Diagnostic subset links receive no credit.

Hover/tap a block for its name, size and percentage. Use arrow keys to select,
Enter to open, and Escape to return to objects. Filters support names/source
paths and constraints such as `camera <70% >10kb`. Zero-size records remain in
the function browser but have no treemap area. The browser smoke test checks
colour, drill-down, each measure, archive filtering and mobile layout; the
synthetic Node test checks proportional area, non-overlap and deterministic
layout.

For an operator rollback, select a retained, previously successful release;
under `.deploy.lock`, restore both `current` and its `state/deployment.json`
metadata from that release's manifest/CI identity. Do not bypass receiver checks
to publish new unverified content. Re-running a successful current-main workflow
also republishes its snapshot. Deployment failures are visible in the GitHub job
summary/log, while the old website continues to serve.

The initial bootstrap used current main `e775bfff81964190939d1679869f89e3de66be86`
and successful CI run `37782401753`: 411 exact functions / 4,864 code bytes,
one 4-byte data allocation, and zero complete replacement linking. The local
historical rebuild agreed with that source checkpoint. Hosting PR changes
supplied the UI/exporter for this bootstrap; subsequent releases come directly
from the main CI artifact.
