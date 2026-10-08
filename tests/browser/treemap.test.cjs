const assert = require('node:assert/strict');
const {layout,color,buildItems,matches,archive} = require('../../tools/web/treemap.js');
const items = Array.from({length:120},(_,i)=>({id:String(i),size:i+1}));
const rectangles=layout(items,900,330);
assert.equal(rectangles.length,120);
assert.deepEqual(rectangles,layout([...items].reverse(),900,330));
assert.deepEqual(layout([{id:'unknown',size:null},{id:'zero',size:0}],900,330),[]);
assert.deepEqual(layout(items,0,330),[]);
const total=items.reduce((n,i)=>n+i.size,0);
for(const r of rectangles) {
 assert.ok(r.w>0&&r.h>0&&r.x>=0&&r.y>=0&&r.x+r.w<=900.000001&&r.y+r.h<=330.000001);
 assert.ok(Math.abs(r.w*r.h/(900*330)-r.size/total)<1e-10);
 for(const b of rectangles) if(b!==r) assert.ok(Math.min(r.x+r.w,b.x+b.w)-Math.max(r.x,b.x)<1e-8 || Math.min(r.y+r.h,b.y+b.h)-Math.max(r.y,b.y)<1e-8);
}
assert.equal(color(0),'rgb(51,58,62)'); assert.equal(color(100),'rgb(31,213,105)');
assert.notEqual(color(50),color(0));assert.notEqual(color(50),color(100));assert.equal(color(-1),color(0));assert.equal(color(Infinity),color(100));
const report={units:[{id:'unity',name:'PiratesIncludeCpp.o',metrics:{total_function_bytes:64,matched_code_percent:50,similarity_percent:75},data_metrics:{total_bytes:16,matched_percent:25},linking:{complete_units:0}},
 {id:'library',name:'libGamebryo.a(TempIncludeCpp.o)',metrics:{total_function_bytes:32,matched_code_percent:0,similarity_percent:99},data_metrics:{total_bytes:0},linking:{complete_units:0}}],
 functions:[{id:'f-a',group_id:'unity',symbol:'duplicate',size:32,status:'matched',byte_verified:true,similarity:100},
 {id:'f-b',group_id:'unity',symbol:'duplicate',size:32,status:'unresolved',byte_verified:false,similarity:100},
 {id:'f-c',group_id:'library',symbol:'camera',size:32,status:'missing',byte_verified:false,similarity:null}],
 data:[{id:'d-a',group_id:'unity',symbol:'data',size:16,status:'matched',byte_verified:true},
 {id:'d-b',group_id:'unowned-data',symbol:'padding',size:32,status:'missing',byte_verified:false}]};
assert.equal(archive(report.units[1]),'libGamebryo.a');
assert.equal(buildItems(report,'exact').find(i=>i.id==='unity').percent,50);
assert.deepEqual(buildItems(report,'exact','unity').map(i=>i.percent),[100,0]);
assert.deepEqual(buildItems(report,'fuzzy','unity').map(i=>i.percent),[100,100]);
assert.equal(buildItems(report,'fuzzy','library')[0].percent,0);
assert.equal(buildItems(report,'exact',null,'libGamebryo.a').length,1);
assert.ok(buildItems(report,'linking').every(i=>i.percent===0));
report.units[0].linking.complete_units=1;
assert.equal(buildItems(report,'linking')[0].percent,100);
assert.equal(buildItems(report,'data').find(i=>i.id==='unowned-data').percent,0);
assert.equal(buildItems(report,'data','unity')[0].percent,100);
const record={name:'camera',id:'f-x',size:12000,percent:69.5,source:'src/game.cpp',groupName:'unity'};
assert.equal(matches(record,'camera <70% >10kb'),true);
assert.equal(matches(record,'camera >=70%'),false);
assert.equal(matches(record,'game >10kib'),true);
assert.equal(matches(record,'unknown'),false);
console.log('PASS: treemap preserves area, boundaries, deterministic layout, duplicate IDs, conservative exact/linking credit, and filters');
