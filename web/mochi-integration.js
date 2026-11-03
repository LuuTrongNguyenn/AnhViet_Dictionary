/* Mochi integration: uses existing C++ Trie API; keeps V7 pixel UI intact. */
(()=>{
'use strict';
let loggedIn=false, pool=[],academyUsed=new Set(),trainingSet=[],trainingIndex=0,trainingCorrect=0;
let activeUser='';
const el=id=>document.getElementById(id);
const api=async (path, params={})=>{const r=await fetch('/api/'+path+'?'+new URLSearchParams(params),{credentials:'same-origin',cache:'no-store'});if(!r.ok)throw Error(r.status===401?'Bạn cần đăng nhập để sử dụng từ điển.':'Không kết nối được với máy chủ C++.');return r.json()};
const escape=s=>String(s).replace(/[&<>"']/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
const storageKey=()=> 'mochi-account-'+activeUser;
const oldUpdate=update;
update=function(){oldUpdate();if(activeUser){localStorage.setItem(storageKey(),JSON.stringify({stage:state.stage,q:state.q,hearts:state.hearts,exp:state.exp,coins:state.coins,step:state.step,used:[...academyUsed].slice(-1500)}));}};
function makeQuestion(item,distractors,kind){const opts=[item,...distractors];let seed=item.word.split('').reduce((a,c)=>a+c.charCodeAt(0),0);for(let i=opts.length-1;i>0;i--){let j=(seed*31+i*17)% (i+1);[opts[i],opts[j]]=[opts[j],opts[i]];seed+=19;}
return {t:kind==='listen'?'🎧 NGHE VÀ CHỌN NGHĨA':'📚 TỪ VỰNG THÁM HIỂM',q:kind==='listen'?'Nghe phát âm và chọn nghĩa chính xác.':'“'+item.word+'” có nghĩa là gì?',audio:kind==='listen'?item.word:null,o:opts.map(x=>x.meaning),a:opts.findIndex(x=>x.word===item.word),word:item.word};}
function populateQuestions(){if(pool.length<320)return;let game=pool.slice(0,300),other=pool.slice(300);let q=game.map((item,i)=>{let d=[];for(let k=0;k<other.length&&d.length<3;k++){let x=other[(i*7+k*23)%other.length];if(x.meaning!==item.meaning&&!d.some(y=>y.meaning===x.meaning))d.push(x)}return makeQuestion(item,d,i%3===1?'listen':'vocab')});questions.splice(0,questions.length,...q);}
async function loadPool(){try{let r=await fetch('/mochi-vocabulary.json',{cache:'no-store'});if(!r.ok)throw Error('missing');pool=await r.json();populateQuestions();}catch(e){console.warn('Không tải được kho câu hỏi mới',e);toast('Không tải được kho từ mới; hãy kiểm tra máy chủ.')}}
function restoreUser(name){activeUser=name;let d={};try{d=JSON.parse(localStorage.getItem(storageKey())||'{}')}catch(e){};academyUsed=new Set(d.used||[]);for(let k of ['stage','q','hearts','exp','coins','step'])if(Number.isFinite(d[k]))state[k]=d[k];state.zone='lobby';update();show('lobby');}
async function authCheck(){if(location.protocol==='file:'){el('mochiAuth').style.display='none';await loadPool();return}try{const r=await api('session');loggedIn=!!r.ok}catch(e){loggedIn=false}el('mochiAuth').style.display=loggedIn?'none':'flex';await loadPool();if(loggedIn){const name=sessionStorage.getItem('mochi-current-user')||'local';restoreUser(name)}}
window.mochiAuthSubmit=async(register)=>{let username=el('mochiUser').value.trim(),password=el('mochiPass').value;if(!username||!password){el('mochiAuthError').textContent='Vui lòng nhập tài khoản và mật khẩu.';return}try{const r=await fetch('/api/'+(register?'register':'login'),{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:new URLSearchParams({username,password}),credentials:'same-origin'});const data=await r.json();if(!data.ok){el('mochiAuthError').textContent=data.message||'Đăng nhập thất bại.';return}loggedIn=true;sessionStorage.setItem('mochi-current-user',username);el('mochiAuth').style.display='none';restoreUser(username);toast('🏰 Chào mừng '+username+' đến với Mochi Kingdom!')}catch(e){el('mochiAuthError').textContent='Không kết nối được với backend C++.'}};
// Real Trie suggestions, rather than fixed 30-word offline list.
let suggestSeq=0;
window.dictSuggest=async()=>{let q=el('dictInput').value.trim().toLowerCase(),seq=++suggestSeq;if(!q){el('dictSuggestions').replaceChildren();return}let words=[];try{if(loggedIn)words=(await api('suggest',{q})).words||[];else words=Object.keys(demoDictionary).filter(w=>w.startsWith(q)).slice(0,7)}catch(e){}if(seq!==suggestSeq)return;el('dictSuggestions').replaceChildren();words.forEach(w=>{let b=document.createElement('button');b.textContent='✦ '+w;b.onclick=()=>dictQuick(w);el('dictSuggestions').appendChild(b)})};
window.dictLookup=async()=>{let w=el('dictInput').value.trim().toLowerCase();if(!w)return;dictLast=w;let meaning=null,source='từ điển mẫu';try{if(loggedIn){let d=await api('lookup',{word:w});if(d.found){meaning=d.meaning;source='Trie C++'}else source='Trie C++ (không tìm thấy)'}else meaning=demoDictionary[w]||null;}catch(e){toast(e.message);return}
let host=el('dictResult');host.replaceChildren();let tag=document.createElement('div');tag.className='dict-kicker';tag.textContent='✦ THƯ VIỆN MA THUẬT ✦';let h=document.createElement('h3');h.textContent=w;let sub=document.createElement('div');sub.className='dict-pron';sub.textContent='English · '+source;let m=document.createElement('div');m.className='dict-meaning';m.textContent=meaning?'📜 '+meaning:'Chưa tìm thấy từ này trong từ điển.';let ctrl=document.createElement('div');ctrl.className='dict-controls';let sp=document.createElement('button');sp.textContent='🔊 Nghe phát âm';sp.onclick=dictSpeak;let sv=document.createElement('button');sv.textContent='📒 Lưu vào sổ tay';sv.onclick=async()=>{dictSave();if(loggedIn)try{await api('add-review',{word:w})}catch(e){console.warn(e)}};ctrl.append(sp,sv);host.append(tag,h,sub,m,ctrl);el('dictSuggestions').replaceChildren();el('dictSource').textContent=loggedIn?'Đang tra từ trực tiếp từ Trie C++ (77.568 từ gốc).':'Đang dùng 30 từ mẫu. Hãy đăng nhập qua ứng dụng để tra toàn bộ từ.'};
// Học viện V10 dùng các chế độ riêng trong index.html; không ghi đè bằng trắc nghiệm từ vựng.
// Guard gameplay against reusing the same question set when API fails.
const originalAsk=ask;
ask=function(){if(pool.length<320){toast('Kho câu hỏi chưa tải xong. Hãy đợi một chút.');return}originalAsk()};
const oldOpenStats=openStats;
openStats=function(){oldOpenStats();el('modalText').textContent='Tiến độ game được lưu trên máy theo tài khoản. Từ điển và Smart Review hoạt động bằng C++.'};
// Offline preview uses the same UI, with sample lookup; Windows integration uses auth.
authCheck();
})();
