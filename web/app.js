const $=id=>document.getElementById(id);const names={dashboard:['Tổng quan','Xin chào, người học 👋','Sẵn sàng chinh phục thêm những từ vựng mới?'],dictionary:['Tra từ điển','Khám phá từ vựng','Tra nghĩa, thêm sửa xóa và gợi ý trực tiếp bằng Trie.'],review:['Smart Review','Ôn tập thông minh 🧠','Ghi nhớ hiệu quả với phương pháp ôn tập cách quãng.'],wrong:['Còn sai','Từ còn sai ✕','Luyện lại những từ bạn chưa nhớ chắc.'],history:['Lịch sử','Lịch sử ôn tập ◷','Xem lại từng phiên ôn: thời gian, câu đúng và câu sai.'],analytics:['Thống kê','Learning Analytics 📊','Theo dõi tần suất hoạt động và tiến độ ghi nhớ.']};let currentWord='',stats={};
async function api(path,params={}){const url='/api/'+path+'?'+new URLSearchParams(params);let res=await fetch(url,{cache:'no-store'});if(!res.ok)throw Error('Không thể kết nối C++ backend');return res.json()}
function toast(msg){$('toast').textContent=msg;$('toast').classList.add('show');setTimeout(()=>$('toast').classList.remove('show'),3000)}
function go(page){document.querySelectorAll('.page').forEach(el=>el.classList.toggle('active',el.id===page));document.querySelectorAll('.nav').forEach(el=>el.classList.toggle('active',el.dataset.page===page));$('crumb').textContent=names[page][0];$('pageTitle').textContent=names[page][1];$('pageSub').textContent=names[page][2];if(page==='review')loadStats();if(page==='wrong')loadWrong();if(page==='history')loadHistory();if(page==='analytics'||page==='dashboard')loadStats()}
document.querySelectorAll('[data-page]').forEach(b=>b.onclick=()=>go(b.dataset.page));document.querySelectorAll('[data-go]').forEach(b=>b.onclick=()=>go(b.dataset.go));$('quickReview').onclick=()=>go('review');
function drawBars(id,values,short=false){let host=$(id);host.replaceChildren();let max=Math.max(1,...values);values.forEach((v,i)=>{let wrap=document.createElement('div');wrap.className='barwrap';let bar=document.createElement('div');bar.className='bar';bar.style.height=Math.max(3,v/max* (short?135:205))+'px';bar.title=v+' lượt ôn';let label=document.createElement('small');label.textContent=short?['T2','T3','T4','T5','T6','T7','CN'][i]:((i+1)%7===0?String(i+1):'');wrap.append(bar,label);host.append(wrap)})}
function drawHeatmap(values){let host=$('heatmap');host.replaceChildren();for(let w=0;w<12;w++){let col=document.createElement('div');col.className='week';for(let d=0;d<7;d++){let v=values[w*7+d]||0;let cell=document.createElement('span');cell.className='level l'+(v===0?0:v<3?1:v<6?2:v<10?3:4);cell.title=v+' lượt ôn';col.append(cell)}host.append(col)}}
async function loadStats(){try{stats=await api('stats');$('sDictionary').textContent=stats.dictionary.toLocaleString('vi-VN');$('sLearned').textContent=stats.learned;$('sWrong').textContent=stats.wrong;$('sStreak').textContent=stats.streak;$('wrongBadge').textContent=stats.wrong;$('aReviews').textContent=stats.reviews;$('aAccuracy').textContent=stats.accuracy+'%';$('aToday').textContent=stats.today;$('aStreak').textContent=stats.streak;$('goalText').textContent=stats.today+' / 10';$('goalFill').style.width=Math.min(100,stats.today*10)+'%';$('goalNote').textContent=stats.today>=10?'Bạn đã hoàn thành mục tiêu hôm nay!':'Còn '+(10-stats.today)+' lượt để hoàn thành mục tiêu.';drawBars('miniChart',stats.days.slice(-7),true);drawBars('largeChart',stats.days);drawHeatmap(stats.heatmap)}catch(e){toast(e.message)}}
let searchToken=0;async function suggest(){let q=$('search').value.trim();let token=++searchToken;if(!q){$('suggestions').innerHTML='<div class="empty">Nhập chữ cái để bắt đầu tìm kiếm.</div>';return}try{let d=await api('suggest',{q});if(token!==searchToken)return;let host=$('suggestions');host.replaceChildren();if(!d.words.length){host.innerHTML='<div class="empty">Không tìm thấy từ phù hợp.</div>';return}d.words.forEach(w=>{let b=document.createElement('button');b.textContent='↗  '+w;b.onclick=()=>lookup(w);host.append(b)})}catch(e){toast(e.message)}}
$('search').addEventListener('input',suggest);$('search').addEventListener('keydown',e=>{if(e.key==='Enter')lookup($('search').value)});
async function lookup(word){try{let d=await api('lookup',{word});currentWord=d.word;$('search').value=d.word;$('wordTitle').textContent=d.word||'Your word';$('meaning').textContent=d.found?d.meaning:'Không tìm thấy từ trong từ điển.';$('addReview').disabled=!d.found;$('addReview').textContent=d.learning?'✓ Đã thêm vào ôn tập':'＋ Thêm vào ôn tập';$('editMeaning').value=d.found?d.meaning:'';$('editPos').value=d.pos||'';$('posBadge').textContent=({noun:'Danh từ · noun',verb:'Động từ · verb',adjective:'Tính từ · adjective',adverb:'Trạng từ · adverb',other:'Loại khác'})[d.pos]||'Chưa phân loại'}catch(e){toast(e.message)}}
$('addReview').onclick=async()=>{let d=await api('add-review',{word:currentWord});toast(d.message);await lookup(currentWord);loadStats()};
async function modify(action){let word=$('search').value.trim(),meaning=$('editMeaning').value.trim();if(!word)return toast('Nhập từ tiếng Anh trước.');if(action==='delete'&&!confirm('Bạn chắc chắn muốn xóa từ '+word+'?'))return;try{let d=await api(action,{word,meaning,pos:$('editPos').value});toast(d.message);if(d.ok){await suggest();await lookup(word);await loadStats()}}catch(e){toast(e.message)}}
$('insertBtn').onclick=()=>modify('insert');$('updateBtn').onclick=()=>modify('update');$('deleteBtn').onclick=()=>modify('delete');

/* ================= SMART REVIEW: trắc nghiệm · điền từ · lật thẻ ================= */
let allWords=[],S=null,lastSetup={};
const MODE_LABEL={quiz:'Trắc nghiệm từ điển','learned-quiz':'Kiểm tra từ đã học',fill:'Điền từ',flash:'Lật thẻ','wrong-quiz':'Trắc nghiệm từ còn sai','wrong-fill':'Điền từ còn sai','wrong-flash':'Lật thẻ từ còn sai','learned-fill':'Điền từ đã học','learned-flash':'Lật thẻ từ đã học'};
const modeLabel=m=>MODE_LABEL[m]||m;
const shuffle=a=>{a=a.slice();for(let i=a.length-1;i>0;i--){let j=Math.floor(Math.random()*(i+1));[a[i],a[j]]=[a[j],a[i]]}return a};
const fmtTime=ts=>new Date(ts*1000).toLocaleString('vi-VN',{hour:'2-digit',minute:'2-digit',day:'2-digit',month:'2-digit',year:'numeric'});
function notice(msg){let n=$('reviewNotice');n.textContent=msg||'';n.hidden=!msg}
async function loadTopics(){try{let d=await api('topics');let sel=$('reviewTopic');sel.replaceChildren();d.topics.forEach(t=>{let o=document.createElement('option');o.value=t;o.textContent=t;sel.append(o)})}catch(e){}}
async function loadAllWords(){if(!allWords.length){let d=await api('all-words');allWords=d.words}return allWords}
function hideBoxes(){['quizBox','fillBox','flashcard','flashActions','resultBox'].forEach(id=>$(id).hidden=true)}
function updateControls(){let m=$('reviewMode').value;$('sourceWrap').hidden=!(m==='fill'||m==='flash');$('dirWrap').hidden=(m==='fill');$('quizNumberWrap').hidden=false}
$('reviewMode').onchange=updateControls;updateControls();

async function poolFor(kind,source,topic){
  let all=await loadAllWords();let pool=all.filter(x=>topic==='Tất cả chủ đề'||x.topic===topic);
  if(source==='learned'){let d=await api('learned-words');let set=new Set(d.words);pool=pool.filter(x=>set.has(x.word))}
  else if(source==='wrong'){let d=await api('wrong-words');let set=new Set(d.words.map(x=>x.word));pool=pool.filter(x=>set.has(x.word))}
  return pool}

async function startReview(){
  try{
    notice('');let m=$('reviewMode').value,topic=$('reviewTopic').value,dir=$('quizDirection').value;
    let kind=m==='learned-quiz'?'quiz':m,source=m==='learned-quiz'?'learned':(kind==='quiz'?'dict':$('reviewSource').value);
    let requested=Number($('quizNumber').value);
    if(!Number.isSafeInteger(requested)||requested<1){notice('Hãy nhập số câu hỏi là số nguyên lớn hơn 0.');$('quizNumber').focus();return}
    let pool=await poolFor(kind,source,topic);
    if(!pool.length){
      notice(source==='learned'?'Chưa có từ đã học'+(topic==='Tất cả chủ đề'?'':' trong chủ đề này')+'. Hãy vào “Tra từ điển”, chọn một từ rồi nhấn “Thêm vào ôn tập”.':source==='wrong'?'Bạn chưa có từ nào còn sai'+(topic==='Tất cả chủ đề'?'':' trong chủ đề này')+'. Tuyệt vời!':'Chủ đề này chưa có từ để ôn.');
      return}
    if(requested>pool.length)notice(`Hiện có ${pool.length} từ phù hợp nên phiên ôn gồm ${pool.length} câu.`);
    let items=shuffle(pool).slice(0,requested);
    let key=source==='dict'?kind:(kind==='quiz'&&source==='learned'?'learned-quiz':source+'-'+kind);
    lastSetup={kind,source,topic,dir};
    beginSession(key,kind,items,dir);
  }catch(e){notice(e.message||'Không thể bắt đầu phiên ôn.')}}
$('startReview').onclick=startReview;

function beginSession(key,kind,items,dir){
  S={key,kind,items,dir,i:0,results:[],sid:Date.now(),answered:false};
  hideBoxes();showQuestion();}
function showQuestion(){
  let it=S.items[S.i];S.answered=false;
  $('reviewCounter').textContent=`Câu ${S.i+1}/${S.items.length} · ${modeLabel(S.key)}`;
  let right=S.results.filter(r=>r.ok).length,head=`Câu ${S.i+1}/${S.items.length} · Đúng ${right}`;
  if(S.kind==='quiz'){
    $('quizBox').hidden=false;$('quizCount').textContent=head;
    $('quizQuestion').textContent=S.dir==='vi-en'?`Từ tiếng Anh nào có nghĩa là “${it.meaning}”?`:`Từ “${it.word}” có nghĩa là gì?`;
    $('quizFeedback').textContent='';$('quizNext').hidden=true;
    let seen=new Set([it.meaning]),others=[];
    for(let tries=0;others.length<3&&tries<400&&allWords.length>1;tries++){let c=allWords[Math.floor(Math.random()*allWords.length)];if(c.word!==it.word&&!seen.has(c.meaning)){seen.add(c.meaning);others.push(c)}}
    let host=$('quizChoices');host.replaceChildren();
    shuffle([it,...others]).forEach(c=>{let b=document.createElement('button');b.textContent=S.dir==='vi-en'?c.word:c.meaning;b.dataset.word=c.word;b.onclick=()=>answerQuiz(b,c,it);host.append(b)});
  }else if(S.kind==='fill'){
    $('fillBox').hidden=false;$('fillCount').textContent=head;
    $('fillPrompt').textContent=it.meaning;$('fillHint').textContent='';$('fillFeedback').textContent='';$('fillFeedback').className='';
    $('fillInput').value='';$('fillInput').disabled=false;$('fillCheck').disabled=false;$('fillHintBtn').disabled=false;$('fillNext').hidden=true;$('fillInput').focus();
  }else{
    $('flashcard').hidden=false;$('flashActions').hidden=false;$('flashCount').textContent=`LẬT THẺ · ${S.i+1}/${S.items.length}`;
    $('flashWord').textContent=S.dir==='vi-en'?it.meaning:it.word;
    $('flashMeaning').textContent=S.dir==='vi-en'?'Hãy nhớ từ tiếng Anh trước khi lật thẻ.':'Hãy nhớ nghĩa tiếng Việt trước khi lật thẻ.';
    $('flipBtn').hidden=false;$('forgotBtn').disabled=true;$('rememberBtn').disabled=true;}}

function record(it,ok){
  S.results.push({word:it.word,meaning:it.meaning,ok});
  api('answer',{word:it.word,mode:S.key,sid:S.sid,ok:ok?1:0}).catch(e=>toast(e.message));}
function nextQuestion(){S.i++;if(S.i<S.items.length)showQuestion();else finishSession()}

/* trắc nghiệm */
function answerQuiz(btn,choice,it){
  if(S.answered)return;S.answered=true;let ok=choice.word===it.word;
  [...$('quizChoices').querySelectorAll('button')].forEach(b=>{b.disabled=true;if(b.dataset.word===it.word)b.classList.add('correct')});
  if(!ok)btn.classList.add('incorrect');
  $('quizFeedback').textContent=ok?'Chính xác! 🎉':'Chưa đúng. Đáp án: '+(S.dir==='vi-en'?it.word:it.meaning);
  record(it,ok);$('quizNext').hidden=false;$('quizNext').textContent=S.i+1===S.items.length?'Xem kết quả':'Câu tiếp theo →'}
$('quizNext').onclick=nextQuestion;

/* điền từ */
function acceptedWords(it){let a=new Set([it.word]);allWords.forEach(x=>{if(x.meaning===it.meaning)a.add(x.word)});return a}
function checkFill(){
  if(!S||S.kind!=='fill'||S.answered)return;let it=S.items[S.i],v=$('fillInput').value.trim().toLowerCase();
  if(!v){$('fillFeedback').textContent='Hãy nhập một từ trước khi kiểm tra.';$('fillFeedback').className='';return}
  S.answered=true;let ok=acceptedWords(it).has(v);
  $('fillInput').disabled=true;$('fillCheck').disabled=true;$('fillHintBtn').disabled=true;
  $('fillFeedback').className=ok?'fbok':'fbbad';
  $('fillFeedback').textContent=ok?(v===it.word?'Chính xác! 🎉':`Đúng! (từ đồng nghĩa trong từ điển; từ gốc: ${it.word})`):`Chưa đúng. Đáp án: ${it.word}`;
  record(it,ok);$('fillNext').hidden=false;$('fillNext').textContent=S.i+1===S.items.length?'Xem kết quả':'Câu tiếp theo →';$('fillNext').focus()}
$('fillCheck').onclick=checkFill;$('fillNext').onclick=nextQuestion;
$('fillHintBtn').onclick=()=>{if(!S||S.answered)return;let w=S.items[S.i].word;$('fillHint').textContent=`Gợi ý: ${w[0]}${' _'.repeat(w.length-1)}  (${w.length} chữ cái)`;$('fillInput').focus()};
$('fillInput').addEventListener('keydown',e=>{if(e.key==='Enter'){e.preventDefault();if(S&&S.answered)nextQuestion();else checkFill()}});

/* lật thẻ */
$('flipBtn').onclick=()=>{if(!S||S.kind!=='flash')return;let it=S.items[S.i];$('flashMeaning').textContent=S.dir==='vi-en'?it.word:it.meaning;$('flipBtn').hidden=true;$('forgotBtn').disabled=false;$('rememberBtn').disabled=false};
function markFlash(ok){if(!S||S.kind!=='flash'||$('flipBtn').hidden===false)return;record(S.items[S.i],ok);nextQuestion()}
$('forgotBtn').onclick=()=>markFlash(false);$('rememberBtn').onclick=()=>markFlash(true);

/* kết quả phiên */
function finishSession(){
  hideBoxes();let total=S.results.length,good=S.results.filter(r=>r.ok).length,bad=S.results.filter(r=>!r.ok);
  $('resultBox').hidden=false;$('reviewCounter').textContent='Đã hoàn thành phiên ôn';
  $('resultTitle').textContent=`Bạn đúng ${good}/${total} câu`;
  $('resultSub').textContent=bad.length?`Có ${bad.length} từ chưa đúng — đã được lưu vào mục “Còn sai”.`:'Tuyệt vời! Bạn nhớ hết các từ trong phiên này.';
  let host=$('resultWrong');host.replaceChildren();
  bad.forEach(r=>{let d=document.createElement('div');d.className='chip bad';let b=document.createElement('b');b.textContent=r.word;let m=document.createElement('span');m.textContent=r.meaning;d.append(b,m);host.append(d)});
  $('retryWrong').hidden=!bad.length;S.retry=bad.map(r=>({word:r.word,meaning:r.meaning}));
  loadStats()}
$('retryWrong').onclick=()=>{if(!S||!S.retry.length)return;let k=S.kind;beginSession('wrong-'+k,k,shuffle(S.retry),S.dir)};
$('resultAgain').onclick=()=>{hideBoxes();$('reviewCounter').textContent='Chọn chế độ rồi nhấn Bắt đầu';notice('')};
$('resultHistory').onclick=()=>go('history');

/* ================= Còn sai ================= */
let wrongItems=[];
async function loadWrong(){
  try{let d=await api('wrong-words');wrongItems=d.words;$('wrongTag').textContent=wrongItems.length+' TỪ';$('wrongBadge').textContent=wrongItems.length;
    let host=$('wrongList');host.replaceChildren();['wrongQuiz','wrongFill','wrongFlash'].forEach(id=>$(id).disabled=!wrongItems.length);
    if(!wrongItems.length){host.innerHTML='<div class="empty">Chưa có từ nào còn sai. Những từ bạn trả lời sai khi ôn tập sẽ xuất hiện ở đây.</div>';return}
    wrongItems.forEach(w=>{let row=document.createElement('div');row.className='wrongrow';
      let info=document.createElement('div');info.className='winfo';let t=document.createElement('b');t.textContent=w.word;let mn=document.createElement('span');mn.textContent=w.meaning;
      let meta=document.createElement('small');meta.textContent=`Sai ${w.count} lần · lần cuối ${fmtTime(w.last)}`+(w.learned?' · đã học':'');info.append(t,mn,meta);
      let btn=document.createElement('button');btn.className='outline';btn.textContent='✓ Đã thuộc';btn.title='Bỏ khỏi danh sách còn sai';
      btn.onclick=async()=>{try{await api('wrong-remove',{word:w.word});await loadWrong();loadStats()}catch(e){toast(e.message)}};
      row.append(info,btn);host.append(row)})
  }catch(e){toast(e.message)}}
async function practiceWrong(kind){
  if(!wrongItems.length)return toast('Chưa có từ còn sai.');
  try{await loadAllWords();go('review');lastSetup={kind,source:'wrong',topic:'Tất cả chủ đề',dir:kind==='fill'?'vi-en':'en-vi'};
    beginSession('wrong-'+kind,kind,shuffle(wrongItems.map(w=>({word:w.word,meaning:w.meaning}))),lastSetup.dir)}catch(e){toast(e.message)}}
$('wrongQuiz').onclick=()=>practiceWrong('quiz');$('wrongFill').onclick=()=>practiceWrong('fill');$('wrongFlash').onclick=()=>practiceWrong('flash');

/* ================= Lịch sử ================= */
async function loadHistory(){
  try{let d=await api('history');$('historyTag').textContent=d.sessions.length+' PHIÊN';let host=$('historyList');host.replaceChildren();
    if(!d.sessions.length){host.innerHTML='<div class="empty">Chưa có lịch sử. Hoàn thành một phiên ôn tập để xem tại đây.</div>';return}
    d.sessions.forEach(s=>{let det=document.createElement('details');det.className='hsession';let sum=document.createElement('summary');
      let wrong=s.total-s.correct,pct=s.total?Math.round(100*s.correct/s.total):0;
      let when=document.createElement('span');when.className='hwhen';when.textContent=fmtTime(s.when);
      let mode=document.createElement('span');mode.className='hmode';mode.textContent=modeLabel(s.mode);
      let score=document.createElement('span');score.className='hscore';score.innerHTML='<i class="ok">✓ '+s.correct+' đúng</i><i class="bad">✕ '+wrong+' sai</i><i>'+pct+'%</i>';
      sum.append(when,mode,score);
      let body=document.createElement('div');body.className='hbody';
      [['Đã đúng',true],['Đã sai',false]].forEach(([title,flag])=>{let col=document.createElement('div');let h=document.createElement('h4');let list=s.items.filter(x=>x.ok===flag);h.textContent=`${title} (${list.length})`;col.append(h);
        if(!list.length){let e=document.createElement('div');e.className='empty';e.textContent='Không có';col.append(e)}
        list.forEach(x=>{let c=document.createElement('div');c.className='chip '+(flag?'ok':'bad');let b=document.createElement('b');b.textContent=x.word;let m=document.createElement('span');m.textContent=x.meaning;c.append(b,m);col.append(c)});body.append(col)});
      det.append(sum,body);host.append(det)})
  }catch(e){toast(e.message)}}

$('quickReview').onclick=()=>go('review');
loadTopics();loadStats();
