/* Original implementation of a byte-weighted binary treemap. No dependencies. */
(function(global) {
'use strict';
const order = (a, b) => b.size - a.size || (a.id < b.id ? -1 : a.id > b.id ? 1 : 0);
const clamp = value => Math.max(0, Math.min(100, Number(value) || 0));
function color(percent, shade = 1) {
  const t = clamp(percent) / 100;
  // Continuous grey -> green, including near matches; equality remains a separate metric.
  const from = [51, 58, 62], to = [31, 213, 105];
  return `rgb(${from.map((v, i) => Math.round((v + (to[i] - v) * t) * shade)).join(',')})`;
}
function layout(items, width, height) {
  const rows = items.filter(i => Number.isFinite(i.size) && i.size > 0).slice().sort(order);
  if (!(width > 0 && height > 0) || !rows.length) return [];
  const result = [], sum = rows.reduce((n, r) => n + r.size, 0);
  function split(start, end, total, x, y, w, h) {
    if (end - start === 1) { result.push({...rows[start], x, y, w, h}); return; }
    let cut = start + 1, left = rows[start].size;
    while (cut < end - 1 && Math.abs(left + rows[cut].size - total / 2) < Math.abs(left - total / 2)) left += rows[cut++].size;
    const ratio = left / total;
    if (w >= h) {
      split(start, cut, left, x, y, w * ratio, h);
      split(cut, end, total - left, x + w * ratio, y, w * (1 - ratio), h);
    } else {
      split(start, cut, left, x, y, w, h * ratio);
      split(cut, end, total - left, x, y + h * ratio, w, h * (1 - ratio));
    }
  }
  split(0, rows.length, sum, 0, 0, width, height);
  return result;
}
function archive(unit) {
  return (unit.name || unit.object_path || '').match(/([^/()]+\.a)\(/)?.[1] || 'Game / application';
}
function buildItems(report, metric, groupId = null, category = '') {
  const data = metric === 'data', units = new Map(report.units.map(u => [u.id, u]));
  const shared = (report.data || []).filter(r => r.group_id === 'unowned-data');
  if (data && shared.length) units.set('unowned-data', {id:'unowned-data', name:'Unattributed data', archive:'Unattributed data',
    data_metrics:{total_bytes:shared.reduce((n,r)=>n+r.size,0), matched_percent:
      100 * shared.reduce((n,r)=>n+(r.byte_verified ? r.size : 0),0) / shared.reduce((n,r)=>n+r.size,0)}});
  const unitPercent = u => clamp(data ? u.data_metrics?.matched_percent : metric === 'fuzzy' ? u.metrics?.similarity_percent :
    metric === 'linking' ? (u.linking?.complete_units ? 100 : 0) : u.metrics?.matched_code_percent);
  if (groupId) {
    const unit = units.get(groupId);
    return (data ? report.data || [] : report.functions).filter(r => r.group_id === groupId && r.size > 0).map(r => ({
      id:r.id, name:r.symbol, size:r.size, groupId, kind:'record', status:r.status,
      percent: metric === 'fuzzy' ? clamp(r.similarity) : metric === 'linking' ? unitPercent(unit || {}) :
        (r.status === 'matched' && r.byte_verified ? 100 : 0), source:r.candidate_source || r.source_path || '',
      groupName:unit?.name || groupId, ambiguities:r.ambiguities || []}));
  }
  return [...units.values()].filter(u => !category || (u.archive || archive(u)) === category).map(u => ({
    id:u.id, name:u.name, size:data ? u.data_metrics?.total_bytes || 0 : u.metrics?.total_function_bytes || 0,
    percent:unitPercent(u), kind:'unit', groupId:u.id, source:u.source || u.source_path || '',
    groupName:u.name, category:u.archive || archive(u)})).filter(i => i.size > 0);
}
function matches(item, query) {
  const text = `${item.name} ${item.id} ${item.source} ${item.groupName}`.toLowerCase();
  return query.toLowerCase().trim().split(/\s+/).filter(Boolean).every(term => {
    const constraint = term.match(/^(<=|>=|<|>)(\d+(?:\.\d+)?)(%|b|kb|mb|kib|mib)$/);
    if (!constraint) return text.includes(term);
    const [,op,n,suffix] = constraint, scale = {b:1,kb:1000,mb:1000000,kib:1024,mib:1048576,'%':1}[suffix];
    const value = suffix === '%' ? item.percent : item.size, target = Number(n) * scale;
    return op === '<' ? value < target : op === '>' ? value > target : op === '<=' ? value <= target : value >= target;
  });
}
function mount(onRecord) {
  const $ = id => document.getElementById(id), canvas = $('progress-map'), ctx = canvas.getContext('2d');
  let report, groupId = null, rectangles = [], active = null, frame;
  const pct = n => clamp(n).toFixed(2) + '%';
  function describe(item) {
    active = item;
    $('map-inspect').hidden = !item;
    $('map-hover').textContent = item ? `${item.name} · ${item.size.toLocaleString()} bytes · ${pct(item.percent)} · ${item.kind === 'unit' ? 'Original object' : item.status}` : 'Hover or tap a block. Enter opens it; arrow keys select another block.';
    $('map-inspect').textContent = item?.kind === 'unit' ? 'Explore object' : 'Inspect comparison';
  }
  function open(item) {
    if (!item) return;
    if (item.kind === 'unit') { groupId = item.groupId; describe(null); draw(); }
    else onRecord(item.id, $('map-metric').value === 'data');
  }
  function draw() {
    if (!report) return;
    const metric = $('map-metric').value, all = buildItems(report, metric, groupId, $('map-category').value);
    const items = all.filter(i => matches(i, $('map-filter').value));
    const width = Math.max(1, canvas.clientWidth), height = Math.max(1, canvas.clientHeight), ratio = window.devicePixelRatio || 1;
    canvas.width = Math.round(width * ratio); canvas.height = Math.round(height * ratio); ctx.setTransform(ratio,0,0,ratio,0,0);
    ctx.fillStyle = '#101619'; ctx.fillRect(0,0,width,height);
    rectangles = layout(items,width,height);
    for (const r of rectangles) {
      const gradient = ctx.createRadialGradient(r.x+r.w*.4,r.y+r.h*.35,0,r.x+r.w*.4,r.y+r.h*.35,Math.max(r.w,r.h));
      gradient.addColorStop(0,color(r.percent)); gradient.addColorStop(1,color(r.percent,.55)); ctx.fillStyle = gradient;
      ctx.fillRect(r.x,r.y,r.w,r.h); ctx.strokeStyle='#101619';ctx.lineWidth=.6;ctx.strokeRect(r.x,r.y,r.w,r.h);
      if (r.w > 105 && r.h > 32) {
        ctx.save();ctx.beginPath();ctx.rect(r.x+5,r.y+3,Math.max(0,r.w-10),Math.max(0,r.h-6));ctx.clip();
        ctx.font='10px system-ui';ctx.fillStyle='#e4e9e9';ctx.fillText(r.name,r.x+6,r.y+15);
        if(r.h > 48) {ctx.fillStyle='#c4d7d0';ctx.fillText(pct(r.percent),r.x+6,r.y+30);}ctx.restore();
      }
    }
    if (!items.length) {ctx.fillStyle='#91a0a7';ctx.font='14px system-ui';ctx.fillText('No non-zero blocks match these filters.',16,32);}
    $('map-back').hidden = !groupId;
    $('map-category').disabled = Boolean(groupId);
    $('map-location').textContent = groupId ? report.units.find(u=>u.id===groupId)?.name || 'Unattributed data' : 'Original compilation objects';
    $('map-summary').textContent = `${items.length.toLocaleString()} ${groupId ? (metric==='data' ? 'allocations' : 'functions') : 'objects'} · ${items.reduce((n,i)=>n+i.size,0).toLocaleString()} bytes shown`;
    $('map-explanation').textContent = metric === 'fuzzy' ? 'Color shows assembly similarity, including unresolved comparisons. It does not prove byte equality. Missing candidates are grey.' :
      metric === 'linking' ? 'Green requires verified complete replacement linking. Diagnostic subset links do not contribute.' :
      metric === 'data' ? 'Color shows verified whole-allocation equality, including padding and zero-fill. Area follows original data bytes.' :
      'Color shows verified byte equality. Object area follows the union of its recovered function ranges; drill-down area follows record sizes, including literal pools.';
    if (active) describe(items.find(i=>i.id===active.id) || null);
  }
  const redraw = () => {cancelAnimationFrame(frame);frame=requestAnimationFrame(draw);};
  function hit(event) {
    const bounds = canvas.getBoundingClientRect(), x = event.clientX-bounds.left, y = event.clientY-bounds.top;
    return rectangles.find(r => x>=r.x && x<r.x+r.w && y>=r.y && y<r.y+r.h);
  }
  canvas.addEventListener('pointermove', event => describe(hit(event)));
  canvas.addEventListener('pointerleave', () => { if(document.activeElement !== canvas) describe(null); });
  canvas.addEventListener('click', event => {const item=hit(event);describe(item);open(item);});
  canvas.addEventListener('keydown', event => {
    if (event.key === 'Enter' || event.key === ' ') {event.preventDefault();open(active || rectangles[0]);}
    else if (event.key.startsWith('Arrow')) {event.preventDefault();const index=rectangles.findIndex(r=>r.id===active?.id);
      describe(rectangles[(index + (event.key==='ArrowLeft'||event.key==='ArrowUp' ? -1 : 1) + rectangles.length) % rectangles.length]);}
    else if (event.key==='Escape' && groupId) {groupId=null;describe(null);draw();}
  });
  $('map-back').onclick=()=>{groupId=null;describe(null);draw();};
  $('map-inspect').onclick=()=>open(active);
  $('map-metric').onchange=()=>{describe(null);draw();};
  $('map-category').onchange=()=>{groupId=null;describe(null);draw();};
  $('map-filter').oninput=()=>{describe(null);draw();};
  new ResizeObserver(redraw).observe(canvas);
  return {update(value) {
    report=value;
    const category=$('map-category').value;
    const categories=[...new Set(report.units.map(archive)), ...(report.data?.some(d=>d.group_id==='unowned-data') ? ['Unattributed data'] : [])].sort();
    $('map-category').replaceChildren(new Option('All original objects',''), ...categories.map(c=>new Option(c,c)));
    if(categories.includes(category)) $('map-category').value=category;
    if(groupId && groupId!=='unowned-data' && !report.units.some(u=>u.id===groupId)) groupId=null;
    redraw();
  }};
}
const api = {layout,color,archive,buildItems,matches,mount};
if (typeof module !== 'undefined' && module.exports) module.exports=api;
else global.PiratesTreemap=api;
})(globalThis);
