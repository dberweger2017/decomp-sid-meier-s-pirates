'use strict';
const $ = id => document.getElementById(id);
const labels = {matched: 'Verified match', different: 'Different', missing: 'Missing candidate', unresolved: 'Unresolved', compile_error: 'Compile error'};
let unitIndex = new Map();
let report, selected = decodeURIComponent(location.hash.slice(1)), selectedFile = '', previousSource = '', dirty = false, requestNumber = 0;
const openGroups = new Set();
const number = n => n.toLocaleString();
const el = (tag, text, cls) => { const e = document.createElement(tag); if (text !== undefined) e.textContent = text; if (cls) e.className = cls; return e; };
async function api(path, options) {
  const response = await fetch(path, options);
  const data = await response.json();
  if (!response.ok) throw new Error(data.error || `Request failed (${response.status})`);
  return data;
}
function error(message) { $('notice').textContent = message; }
function state(building, code = 0) {
  $('build-state').textContent = building ? '● Rebuilding…' : code ? '● Build failed · diagnostics retained' : '● Watching for edits';
  $('build-state').style.color = code ? 'var(--red)' : building ? 'var(--gold)' : 'var(--green)';
}
function renderMetrics() {
  const m = report.metrics;
  $('matched-bytes').textContent = number(m.matched_bytes);
  $('total-bytes').textContent = `of ${number(m.total_function_bytes)} recovered function bytes`;
  $('matched-functions').textContent = number(m.matched_functions);
  $('total-functions').textContent = `of ${number(m.total_functions)} function records`;
  $('missing').textContent = number(m.missing_candidates);
  $('unresolved').textContent = number(m.unresolved_comparisons);
  $('errors').textContent = `${number(m.compile_errors)} compile errors`;
  $('similarity').textContent = m.compared_similarity_percent === null ? '—' : `${m.compared_similarity_percent.toFixed(1)}%`;
  $('input-hash').textContent = `INPUT SHA-256 ${report.input_sha256.slice(0, 16)}…`;
  $('group-count').textContent = `${report.units.length} groups`;
  $('notice').textContent = report.kind === 'synthetic' ? 'SYNTHETIC FIXTURE · Modern Clang tests the tooling. These matches are not game source progress.' : report.compiler.validated ? 'Historical cross-build validated. Exact original compiler revision and flags remain unproven.' : 'HISTORICAL COMPILER UNVALIDATED · ' + (report.compiler.reason || 'Missing validation evidence') + ' · Source-matching progress starts at zero.';
}
function renderTree() {
  const q = $('search').value.toLowerCase(), status = $('status').value, source = $('source-filter').value.toLowerCase();
  const byGroup = new Map();
  if (!q && status === 'all' && !source) report.units.forEach(u => byGroup.set(u.id, []));
  let count = 0;
  for (const f of report.functions) {
    const u = unitIndex.get(f.group_id);
    const haystack = `${f.symbol} ${u?.name || ''} ${u?.object_path || ''}`.toLowerCase();
    const paths = `${f.source_path} ${f.candidate_source || ''} ${u?.source_path || ''}`.toLowerCase();
    if ((q && !haystack.includes(q)) || (status !== 'all' && f.status !== status) || (source && !paths.includes(source))) continue;
    if (!byGroup.has(f.group_id)) byGroup.set(f.group_id, []);
    byGroup.get(f.group_id).push(f); count++;
  }
  $('result-count').textContent = `${number(count)} functions in ${byGroup.size} original objects`;
  const tree = $('tree'); tree.replaceChildren();
  for (const unit of report.units) {
    const funcs = byGroup.get(unit.id); if (!funcs) continue;
    const group = el('details', undefined, 'object'); group.dataset.groupId = unit.id;
    group.open = openGroups.has(unit.id) || (q && byGroup.size <= 12);
    const summary = el('summary', unit.name); summary.title = unit.object_path;
    summary.append(el('span', number(funcs.length))); group.append(summary);
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
  const f = report.functions.find(f => f.id === id);
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
    $('function-meta').textContent = `0x${f.address.toString(16).padStart(8, '0')} · ${f.mode.toUpperCase()} · ${f.size} bytes · Verified equality: ${f.byte_verified ? 'yes' : 'no'} · Similarity: ${f.similarity === null ? 'not compared' : f.similarity + '%'}`;
    $('function-status').textContent = labels[f.status]; $('function-status').className = `badge ${f.status}`;
    $('reasons').textContent = f.reasons.join(' · ');
    $('original-size').textContent = `${f.size} bytes`;
    $('candidate-size').textContent = f.candidate_size === null ? 'Missing' : `${f.candidate_size} bytes`;
    const assembly = $('assembly'); assembly.replaceChildren();
    const hasCandidate = f.rows.some(r => r.candidate);
    f.rows.forEach((r, i) => { const row = el('div', undefined, 'asm-row');
      row.append(asmCell(r.original, r.differences, hasCandidate, false), asmCell(r.candidate, r.differences, hasCandidate, i === 0)); assembly.append(row); });
    const relocations = $('relocations'); relocations.replaceChildren();
    if (f.relocations.length) { relocations.append(el('h3', 'Relocation targets · no address masking'));
      f.relocations.forEach(r => relocations.append(el('pre', `+0x${r.offset.toString(16)} · type ${r.type} · ${r.status}\n${r.target || r.reason}${r.target_address === undefined ? '' : ' → 0x' + r.target_address.toString(16)}\n${r.before || '?'} → ${r.after || '?'}`))); }
    $('compiler').textContent = JSON.stringify({compiler: f.compiler, flags: f.compile?.flags || unit?.flags || [], source: f.candidate_source, ambiguities: f.ambiguities}, null, 2);
    $('diagnostics').textContent = f.diagnostic_text || (f.candidate_source ? 'No compiler diagnostics.' : 'No candidate compilation configured for this object.');
    await loadFiles(f);
  } catch (e) { if (serial === requestNumber) error(e.message); }
}
async function loadFiles(f) {
  const files = await api('/api/files?unit=' + encodeURIComponent(f.group_id));
  if (f.id !== selected) return;
  $('no-source').hidden = files.length > 0; $('editor').hidden = files.length === 0;
  if (!files.length) return;
  const selector = $('source-file'); selector.replaceChildren();
  files.forEach(path => { const option = el('option', path); option.value = path; selector.append(option); });
  selector.value = files.includes(selectedFile) ? selectedFile : files.includes(f.candidate_source) ? f.candidate_source : files[0];
  if (!dirty) await loadSource(f.group_id, selector.value);
}
async function loadSource(unit, path) {
  try { const data = await api(`/api/source?unit=${encodeURIComponent(unit)}&path=${encodeURIComponent(path)}`);
    if (!report.functions.some(f => f.id === selected && f.group_id === unit) || $('source-file').value !== path) return;
    selectedFile = path; previousSource = data.content; $('source-content').value = data.content; dirty = false; $('save-status').textContent = '';
  } catch (e) { error(e.message); }
}
$('source-content').addEventListener('input', () => { dirty = true; $('save-status').textContent = 'Unsaved changes'; });
$('source-file').addEventListener('change', () => {
  if (dirty) { $('source-file').value = selectedFile; error('Save your source changes before opening another file.'); return; }
  const f = report.functions.find(f => f.id === selected); loadSource(f.group_id, $('source-file').value);
});
$('save').onclick = async () => {
  const f = report.functions.find(f => f.id === selected), content = $('source-content').value;
  try { await api('/api/source', {method: 'POST', headers: {'Content-Type': 'application/json', 'X-Pirates-Token': window.editToken},
    body: JSON.stringify({unit: f.group_id, path: selectedFile, content, previous: previousSource})});
    previousSource = content; dirty = false; $('save-status').textContent = 'Saved · rebuild queued';
  } catch (e) { $('save-status').textContent = e.message; }
};
for (const button of document.querySelectorAll('[data-tab]')) button.onclick = () => {
  for (const tab of document.querySelectorAll('[data-tab]')) { const active = tab === button;
    tab.setAttribute('aria-selected', String(active)); $(tab.dataset.tab + '-panel').hidden = !active; }
};
for (const id of ['search', 'status', 'source-filter']) $(id).addEventListener('input', renderTree);
async function refresh() {
  try { report = await api('/api/report'); unitIndex = new Map(report.units.map(u => [u.id, u])); renderMetrics();
    if (!report.functions.some(f => f.id === selected)) selected = report.functions[0]?.id || '';
    const f = report.functions.find(f => f.id === selected); if (f) openGroups.add(f.group_id);
    renderTree(); await renderFunction();
  } catch (e) { error(e.message); }
}
const events = new EventSource('/api/events');
events.onmessage = async message => { const event = JSON.parse(message.data);
  if (event.kind === 'building') state(true);
  else if (event.kind === 'built') { state(false, event.returncode); await refresh();
    if (event.returncode) $('notice').textContent += ' · ' + event.output.slice(-600); }
  else if (event.kind === 'report') await refresh();
};
events.onerror = () => { $('build-state').textContent = '● Reconnecting to watcher…'; };
refresh(); api('/api/status').then(s => state(s.building, s.returncode)).catch(e => error(e.message));
