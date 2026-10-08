(() => {
'use strict';
const $ = id => document.getElementById(id);
const hosted = window.piratesHosted === true;
let snapshot, hostedSources;
const labels = {matched: 'Verified match', different: 'Different', missing: 'Missing candidate', unresolved: 'Unresolved', compile_error: 'Compile error'};
let unitIndex = new Map();
let report, selected = decodeURIComponent(location.hash.slice(1)), selectedFile = '', previousSource = '', dirty = false, requestNumber = 0;
const openGroups = new Set();
const number = n => n.toLocaleString();
const percent = n => n === null || n === undefined ? "N/A" : n.toFixed(4) + "%";
const records = () => $("record-kind").value === "data" ? (report.data || []) : report.functions;
const selectedRecord = () => [...report.functions, ...(report.data || [])].find(f => f.id === selected);
const el = (tag, text, cls) => { const e = document.createElement(tag); if (text !== undefined) e.textContent = text; if (cls) e.className = cls; return e; };
async function api(path, options) {
  if (hosted) {
    if (options?.method && options.method !== 'GET') throw new Error('This published snapshot is read-only.');
    const url = new URL(path, location.origin);
    const base = '/releases/' + snapshot.release + '/';
    if (url.pathname === '/api/report') path = base + 'report.json';
    else if (url.pathname === '/api/function') path = base + 'functions/' + encodeURIComponent(url.searchParams.get('id')) + '.json';
    else if (url.pathname === '/api/link') path = base + 'link.json';
    else if (url.pathname === '/api/files' || url.pathname === '/api/source') {
      hostedSources ||= await api('/sources.json');
      if (url.pathname === '/api/files') return hostedSources.files[url.searchParams.get('unit')] || [];
      const source = url.searchParams.get('path');
      if (!Object.hasOwn(hostedSources.sources, source)) throw new Error('Source not included in this snapshot');
      return {path: source, content: hostedSources.sources[source]};
    } else if (path === '/sources.json') path = base + 'sources.json';
    else throw new Error('API unavailable in a published snapshot');
  }
  const response = await fetch(path, options);
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || `Request failed (${response.status})`);
  return data;
}
function error(message) { $('notice').textContent = message; }
function state(building, code = 0) {
  if (hosted) return;
  $('build-state').textContent = building ? '● Rebuilding…' : code ? '● Build failed · diagnostics retained' : '● Watching for edits';
  $('build-state').style.color = code ? 'var(--red)' : building ? 'var(--gold)' : 'var(--green)';
}
function renderMetrics() {
  const m = report.metrics;
  $('matched-bytes').textContent = percent(m.matched_code_percent);
  $('total-bytes').textContent = `${number(m.matched_bytes)} / ${number(m.total_function_bytes)} function bytes`;
  $('matched-functions').textContent = percent(m.matched_functions_percent);
  $('total-functions').textContent = `${number(m.matched_functions)} / ${number(m.total_functions)} records`;
  $('missing').textContent = number(m.missing_candidates);
  $('unresolved').textContent = number(m.unresolved_comparisons);
  $('errors').textContent = `${number(m.compile_errors)} compile errors`;
  $('similarity').textContent = percent(m.similarity_percent);
  $('compared-similarity').textContent = `Compared candidates: ${percent(m.compared_similarity_percent)}`;
  const dm = report.data_metrics;
  $('matched-data').textContent = percent(dm?.matched_percent);
  $('total-data').textContent = dm ? `${number(dm.matched_bytes)} / ${number(dm.total_bytes)} bytes · ${number(dm.unresolved)} unresolved` : 'Not inventoried';
  const link = report.linking;
  $('linked-units').textContent = `${link.complete_units || 0} / ${report.units.length}`;
  $('link-state').textContent = link.state || 'unsupported';
  $('link-footer').textContent = `Full-game linking: ${link.state || 'unsupported'} · ${link.complete_units || 0} complete units`;
  $('link-reason').textContent = link.reason;
  $('link-details').textContent = JSON.stringify(link, null, 2);
  $('progress-scope').textContent = 'Code: recovered function ranges, including literal pools. Missing source contributes zero fuzzy progress. Data: whole non-code allocations, including padding and zero-fill. Structural linking and verified image equality are separate.';
  $('input-hash').textContent = `INPUT SHA-256 ${report.input_sha256.slice(0, 16)}…`;
  $('group-count').textContent = `${report.units.length} groups`;
  $('notice').textContent = report.kind === 'synthetic' ? 'SYNTHETIC FIXTURE · Modern Clang tests the tooling. These matches are not game source progress.' : report.compiler.validated ? 'Historical cross-build validated. Exact original compiler revision and flags remain unproven.' : 'HISTORICAL COMPILER UNVALIDATED · ' + (report.compiler.reason || 'Missing validation evidence') + ' · Source-matching progress starts at zero.';
}
function renderTree() {
  if (!report) return;
  const q = $('search').value.toLowerCase(), status = $('status').value, source = $('source-filter').value.toLowerCase();
  const byGroup = new Map();
  if ($('record-kind').value === 'code' && !q && status === 'all' && !source) report.units.forEach(u => byGroup.set(u.id, []));
  let count = 0;
  for (const f of records()) {
    const u = unitIndex.get(f.group_id);
    const haystack = `${f.symbol} ${u?.name || ''} ${u?.object_path || ''}`.toLowerCase();
    const paths = `${f.source_path} ${f.candidate_source || ''} ${u?.source_path || ''}`.toLowerCase();
    if ((q && !haystack.includes(q)) || (status !== 'all' && f.status !== status) || (source && !paths.includes(source))) continue;
    if (!byGroup.has(f.group_id)) byGroup.set(f.group_id, []);
    byGroup.get(f.group_id).push(f); count++;
  }
  $('result-count').textContent = `${number(count)} ${$("record-kind").value === "data" ? "data allocations" : "functions"} in ${byGroup.size} groups`;
  const tree = $('tree'); tree.replaceChildren();
  for (const unit of [...report.units, {id: "unowned-data", name: "Unattributed data", object_path: "Original section allocations without unique STABS ownership"}]) {
    const funcs = byGroup.get(unit.id); if (!funcs) continue;
    const group = el('details', undefined, 'object'); group.dataset.groupId = unit.id;
    group.open = openGroups.has(unit.id) || (q && byGroup.size <= 12);
    const summary = el('summary', unit.name); summary.title = unit.object_path;
    summary.append(el('span', number(funcs.length))); group.append(summary);
    const m = unit.metrics, dm = unit.data_metrics;
    if (m) { const progress = el('div', undefined, 'unit-progress');
      const bar = el('progress'); bar.max = 100; bar.value = m.matched_code_percent || 0;
      bar.title = `Exact code: ${percent(m.matched_code_percent)} · Fuzzy code: ${percent(m.similarity_percent)}`;
      progress.append(bar, el('small', `Exact ${percent(m.matched_code_percent)} · Fuzzy ${percent(m.similarity_percent)} · Data ${percent(dm?.matched_percent)}`)); group.append(progress); }
    const children = el('div'); group.append(children);
    let rendered = false;
    function fill() {
      if (rendered) return; rendered = true;
      for (const f of funcs) {
        const button = el('button', undefined, 'function' + (selected === f.id ? ' selected' : ''));
        button.dataset.functionId = f.id; button.title = `${f.symbol}\n${f.id} · ${labels[f.status]}`;
        button.setAttribute('role', 'treeitem'); button.setAttribute('aria-selected', String(selected === f.id));
        button.append(el('span', undefined, `dot ${f.status}`), el('span', f.symbol, 'name'));
        button.onclick = () => selectFunction(f.id);
        children.append(button);
      }
    }
    group.addEventListener('toggle', () => { if (group.open) { openGroups.add(unit.id); fill(); } else openGroups.delete(unit.id); });
    if (group.open) fill();
    tree.append(group);
  }
  if (!count) tree.append(el('p', 'No functions match these filters.', 'subtle'));
}
async function selectFunction(id) {
  if (id !== selected && dirty) {
    // Keep the editor content while navigation is pending; avoid silently
    // discarding unsaved work when the watcher refreshes the comparison.
    error('Save your source changes before selecting another function.'); return;
  }
  selected = id; history.replaceState(null, '', '#' + encodeURIComponent(id));
  const f = [...report.functions, ...(report.data || [])].find(f => f.id === id);
  if (f) openGroups.add(f.group_id);
  renderTree();
  await renderFunction();
}
function asmCell(value, changes, hasCandidate, first) {
  const cell = el('div', undefined, 'asm-cell');
  if (!value) { cell.classList.add('empty'); if (!hasCandidate && first) { cell.classList.add('no-candidate'); cell.textContent = 'No compiled candidate'; } return cell; }
  for (const [key, text, cls] of [ ['address', value.address.toString(16).padStart(8, '0') + '  ', 'addr'],
      ['bytes', value.bytes.padEnd(10) + ' ', 'bytes'], ['instruction', value.instruction.padEnd(10) + ' ', 'instruction'],
      ['operands', value.operands, 'operands'] ]) {
    const span = el('span', text, cls + (hasCandidate && changes.includes(key) ? ' changed' : ''));
    cell.append(span);
  }
  if (changes.includes('registers') && hasCandidate) cell.title = 'Register difference: ' + value.registers.join(', ');
  return cell;
}
async function renderFunction() {
  if (!selected) return;
  const serial = ++requestNumber;
  try {
    const f = await api('/api/function?id=' + encodeURIComponent(selected));
    if (serial !== requestNumber || f.id !== selected) return;
    $('empty').hidden = true; $('function-view').hidden = false;
    const unit = report.units.find(u => u.id === f.group_id);
    $('function-group').textContent = `${unit?.name || f.group_id} / ${f.id}`;
    $('function-name').textContent = f.symbol;
    $('function-meta').textContent = `0x${f.address.toString(16).padStart(8, '0')} · ${(f.mode || (f.zerofill ? 'zero-fill data' : 'data')).toUpperCase()} · ${f.size} bytes · Verified equality: ${f.byte_verified ? 'yes' : 'no'} · Similarity: ${f.similarity === null ? 'not compared' : f.similarity + '%'}`;
    $('function-status').textContent = labels[f.status]; $('function-status').className = `badge ${f.status}`;
    $('reasons').textContent = f.reasons.join(' · ');
    $('original-size').textContent = `${f.size} bytes`;
    $('candidate-size').textContent = f.candidate_size === null ? 'Missing' : `${f.candidate_size} bytes`;
    const assembly = $('assembly'); assembly.replaceChildren();
    $('data-format').hidden = !f.id.startsWith('d-');
    if (f.id.startsWith('d-')) {
      if (f.zerofill) assembly.append(el('pre', `Zero-fill allocation · ${f.size} bytes · alignment ${f.alignment}
Candidate allocation: ${f.candidate_size ?? 'missing'} · equality requires full size and alignment.`));
      else f.rows.forEach(r => { const row = el('div', undefined, 'asm-row');
        const suffix = $('data-format').value === 'hex' ? '' : '_' + $('data-format').value;
        row.append(el('div', `+0x${r.offset.toString(16)}  ${r['original' + suffix]}`, 'asm-cell'), el('div', r['candidate' + suffix] || 'No compiled candidate', 'asm-cell' + (r.different ? ' changed' : ''))); assembly.append(row); });
      if (f.display_truncated) assembly.append(el('p', 'Display limited to 4096 bytes; equality includes the whole allocation.', 'subtle'));
    } else {
      const hasCandidate = f.rows.some(r => r.candidate);
      f.rows.forEach((r, i) => { const row = el('div', undefined, 'asm-row');
        row.append(asmCell(r.original, r.differences, hasCandidate, false), asmCell(r.candidate, r.differences, hasCandidate, i === 0)); assembly.append(row); });
    }
    const relocations = $('relocations'); relocations.replaceChildren();
    if (f.relocations.length) { relocations.append(el('h3', 'Relocation targets · no address masking'));
      f.relocations.forEach(r => relocations.append(el('pre', `+0x${r.offset.toString(16)} · type ${r.type} · ${r.status}\n${r.target || r.reason}${r.target_address === undefined ? '' : ' → 0x' + r.target_address.toString(16)}\n${r.before || '?'} → ${r.after || '?'}`))); }
    $('compiler').textContent = JSON.stringify({compiler: f.compiler, flags: f.compile?.flags || unit?.flags || [], source: f.candidate_source, ambiguities: f.ambiguities, boundary: f.boundary, alignment: f.alignment, aliases: f.aliases}, null, 2);
    $('diagnostics').textContent = f.diagnostic_text || (f.candidate_source ? 'No compiler diagnostics.' : 'No candidate compilation configured for this object.');
    await loadFiles(f);
  } catch (e) { if (serial === requestNumber) error(e.message); }
}
async function loadFiles(f) {
  const files = await api('/api/files?unit=' + encodeURIComponent(f.candidate_group_id || f.group_id));
  if (f.id !== selected) return;
  $('no-source').hidden = files.length > 0; $('editor').hidden = files.length === 0;
  if (!files.length) return;
  const selector = $('source-file'); selector.replaceChildren();
  files.forEach(path => { const option = el('option', path); option.value = path; selector.append(option); });
  selector.value = files.includes(selectedFile) ? selectedFile : files.includes(f.candidate_source) ? f.candidate_source : files[0];
  if (!dirty) await loadSource(f.candidate_group_id || f.group_id, selector.value);
}
async function loadSource(unit, path) {
  try { const data = await api(`/api/source?unit=${encodeURIComponent(unit)}&path=${encodeURIComponent(path)}`);
    if (!selectedRecord() || (selectedRecord().candidate_group_id || selectedRecord().group_id) !== unit || $('source-file').value !== path) return;
    selectedFile = path; previousSource = data.content; $('source-content').value = data.content; dirty = false; $('save-status').textContent = '';
  } catch (e) { error(e.message); }
}
$('source-content').addEventListener('input', () => { dirty = true; $('save-status').textContent = 'Unsaved changes'; });
if (hosted) { $('source-content').readOnly = true; $('save').hidden = true; $('source-content').setAttribute('aria-label', 'Candidate source (read-only)'); }
$('source-file').addEventListener('change', () => {
  if (dirty) { $('source-file').value = selectedFile; error('Save your source changes before opening another file.'); return; }
  const f = selectedRecord(); loadSource(f.candidate_group_id || f.group_id, $('source-file').value);
});
$('save').onclick = async () => {
  const f = selectedRecord(), content = $('source-content').value;
  try { await api('/api/source', {method: 'POST', headers: {'Content-Type': 'application/json', 'X-Pirates-Token': window.editToken},
    body: JSON.stringify({unit: f.candidate_group_id || f.group_id, path: selectedFile, content, previous: previousSource})});
    previousSource = content; dirty = false; $('save-status').textContent = 'Saved · rebuild queued';
  } catch (e) { $('save-status').textContent = e.message; }
};
for (const button of document.querySelectorAll('[data-tab]')) button.onclick = () => {
  for (const tab of document.querySelectorAll('[data-tab]')) { const active = tab === button;
    tab.setAttribute('aria-selected', String(active)); $(tab.dataset.tab + '-panel').hidden = !active; }
};
for (const id of ['search', 'status', 'source-filter']) $(id).addEventListener('input', renderTree);
$('data-format').onchange = renderFunction;
async function linkDiagnostics() {
  try { const link = await api('/api/link'); $('link-diagnostics').textContent = link.diagnostic_text || 'No linker diagnostics. ' + link.reason; }
  catch(e) { error(e.message); }
}
$('show-linking').onclick = async () => { $('linking-panel').hidden = !$('linking-panel').hidden; if (!$('linking-panel').hidden) await linkDiagnostics(); };
$('record-kind').onchange = () => { if (dirty) { $('record-kind').value = selected.startsWith('d-') ? 'data' : 'code'; error('Save your source changes before changing views.'); return; } selected = ''; renderTree(); $('function-view').hidden = true; $('empty').hidden = false; };
async function refresh() {
  try { report = await api('/api/report'); unitIndex = new Map(report.units.map(u => [u.id, u])); renderMetrics();
    if (!selectedRecord()) selected = records()[0]?.id || '';
    const f = selectedRecord(); if (f) { $('record-kind').value = f.id.startsWith('d-') ? 'data' : 'code'; openGroups.add(f.group_id); }
    renderTree(); await renderFunction(); if (!$('linking-panel').hidden) await linkDiagnostics();
  } catch (e) { error(e.message); }
}
if (!hosted) {
const events = new EventSource('/api/events');
events.onmessage = async message => { const event = JSON.parse(message.data);
  if (event.kind === 'building') state(true);
  else if (event.kind === 'built') { state(false, event.returncode); await refresh();
    if (event.returncode) $('notice').textContent += ' · ' + event.output.slice(-600); }
  else if (event.kind === 'report') await refresh();
};
events.onerror = () => { $('build-state').textContent = '● Reconnecting to watcher…'; };
refresh(); api('/api/status').then(s => state(s.building, s.returncode)).catch(e => error(e.message));
} else {
  let checking = false;
  async function checkSnapshot() {
    if (checking) return;
    checking = true;
    try {
      const response = await fetch('/deployment.json', {cache: 'no-store'});
      if (!response.ok) throw new Error('Published snapshot unavailable');
      const next = await response.json();
      if (next.release !== snapshot?.release) {
        if (snapshot) { location.reload(); return; } // preserve hash and load the release's JS as well as its data
        snapshot = next;
        const link = el('a', next.commit.slice(0, 12));
        link.href = 'https://github.com/dberweger2017/decomp-sid-meier-s-pirates/commit/' + next.commit;
        $('build-state').replaceChildren(el('span', 'Read-only · '), link);
        const stamp = el('span', ` · ${next.published_at || next.updated_at}`); $('build-state').append(stamp);
        $('build-state').title = 'Published after successful CI; checks for updates every 30 seconds.';
        await refresh();
      }
    } catch (e) { error(e.message); }
    finally { checking = false; }
  }
  checkSnapshot(); setInterval(checkSnapshot, 30000);
}
})();
