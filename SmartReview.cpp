#include "../include/SmartReview.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <set>
namespace {
std::tm localDay(std::time_t t) { std::tm out{};
#ifdef _WIN32
localtime_s(&out,&t);
#else
localtime_r(&t,&out);
#endif
return out; }
std::string dayKey(std::time_t t) {auto d=localDay(t); char b[16];std::strftime(b,sizeof(b),"%Y-%m-%d",&d);return b;}
std::time_t midnight(std::time_t t){auto d=localDay(t);d.tm_hour=12;d.tm_min=0;d.tm_sec=0;return std::mktime(&d);}
std::time_t offsetDay(std::time_t now,int offset){auto d=localDay(now);d.tm_hour=12;d.tm_min=0;d.tm_sec=0;d.tm_mday+=offset;return std::mktime(&d);}
bool validWord(const std::string& s){if(s.empty())return false;for(char c:s)if(c<'a'||c>'z')return false;return true;}
}
bool SmartReview::add(const std::string& word){if(!validWord(word))return false;auto p=words.emplace(word,ReviewProgress{});if(p.second){p.first->second.word=word;p.first->second.due=std::time(nullptr);}return p.second;}
bool SmartReview::contains(const std::string& word) const{return words.find(word)!=words.end();}
bool SmartReview::remove(const std::string& word){wrong.erase(word);return words.erase(word)>0;}
bool SmartReview::review(const std::string& word,bool remembered,std::time_t now){auto it=words.find(word);if(it==words.end())return false;auto& p=it->second;if(remembered){p.correct++;p.streak++;static const int intervals[]={1,3,7,14,30};p.interval=intervals[std::min(p.streak-1,4)];}else{p.wrong++;p.streak=0;p.interval=1;}p.due=offsetDay(now,p.interval);events.push_back({word,now,remembered});return true;}
bool SmartReview::recordAnswer(long long sid,const std::string& mode,const std::string& word,bool correct,std::time_t now){
if(!validWord(word)||mode.empty()||mode.size()>16)return false;
for(char ch:mode)if(!((ch>='a'&&ch<='z')||ch=='-'))return false;
if(contains(word))review(word,correct,now);else events.push_back({word,now,correct});
records.push_back({sid,now,mode,word,correct});
if(correct)wrong.erase(word);else{auto& w=wrong[word];w.count++;w.last=now;}
return true;}
std::vector<std::string> SmartReview::due(std::time_t now) const{std::vector<std::pair<std::time_t,std::string>> a;for(const auto& kv:words)if(kv.second.due<=now)a.push_back({kv.second.due,kv.first});std::sort(a.begin(),a.end());std::vector<std::string> out;for(auto& p:a)out.push_back(p.second);return out;}
int SmartReview::correctReviews() const{int n=0;for(const auto& e:events)if(e.remembered)n++;return n;}
int SmartReview::todayReviews(std::time_t now) const{int n=0;auto k=dayKey(now);for(const auto& e:events)if(dayKey(e.when)==k)n++;return n;}
std::vector<int> SmartReview::lastDays(int days,std::time_t now) const{std::vector<int> a(std::max(0,days));std::map<std::string,int> count;for(auto& e:events)count[dayKey(e.when)]++;for(int i=0;i<days;i++)a[i]=count[dayKey(offsetDay(now,i-days+1))];return a;}
int SmartReview::streakDays(std::time_t now) const{std::set<std::string> active;for(auto& e:events)active.insert(dayKey(e.when));int n=0;int start=active.count(dayKey(now))?0:-1;for(int i=start;i>-50000;i--){if(!active.count(dayKey(offsetDay(now,i))))break;n++;}return n;}
bool SmartReview::load(const std::string& folder){words.clear();events.clear();records.clear();wrong.clear();std::ifstream in(folder+"/learning_progress.txt",std::ios::binary);std::string line;while(std::getline(in,line)){std::istringstream s(line);ReviewProgress p;long long due=0;if(std::getline(s,p.word,'\t') && (s>>p.correct>>p.wrong>>p.streak>>p.interval>>due) && validWord(p.word)){p.due=(std::time_t)due;words[p.word]=p;}}std::ifstream log(folder+"/review_history.csv",std::ios::binary);while(std::getline(log,line)){std::istringstream s(line);std::string word,ts,ok;if(std::getline(s,word,',')&&std::getline(s,ts,',')&&std::getline(s,ok) && validWord(word)){try{long long t=std::stoll(ts);if(ok=="0"||ok=="1")events.push_back({word,(std::time_t)t,ok=="1"});}catch(...){}}}
std::ifstream wf(folder+"/wrong_words.txt",std::ios::binary);while(std::getline(wf,line)){std::istringstream s(line);std::string w;WrongInfo i;long long t=0;if(std::getline(s,w,'\t')&&(s>>i.count>>t)&&validWord(w)&&i.count>0){i.last=(std::time_t)t;wrong[w]=i;}}
std::ifstream qf(folder+"/quiz_history.csv",std::ios::binary);while(std::getline(qf,line)){if(!line.empty()&&line.back()=='\r')line.pop_back();std::istringstream s(line);std::string sid,ts,mode,word,ok;if(std::getline(s,sid,',')&&std::getline(s,ts,',')&&std::getline(s,mode,',')&&std::getline(s,word,',')&&std::getline(s,ok)&&validWord(word)&&(ok=="0"||ok=="1")){try{records.push_back({std::stoll(sid),(std::time_t)std::stoll(ts),mode,word,ok=="1"});}catch(...){}}}
return true;}
bool SmartReview::save(const std::string& folder) const{std::ofstream out(folder+"/learning_progress.txt",std::ios::binary|std::ios::trunc);if(!out)return false;for(auto& kv:words){auto& p=kv.second;out<<p.word<<'\t'<<p.correct<<'\t'<<p.wrong<<'\t'<<p.streak<<'\t'<<p.interval<<'\t'<<(long long)p.due<<'\n';}out.close();if(!out)return false;std::ofstream log(folder+"/review_history.csv",std::ios::binary|std::ios::trunc);if(!log)return false;for(auto& e:events)log<<e.word<<','<<(long long)e.when<<','<<(e.remembered?1:0)<<'\n';log.close();if(!log)return false;
std::ofstream wf(folder+"/wrong_words.txt",std::ios::binary|std::ios::trunc);if(!wf)return false;for(auto& kv:wrong)wf<<kv.first<<'\t'<<kv.second.count<<'\t'<<(long long)kv.second.last<<'\n';wf.close();if(!wf)return false;
std::ofstream qf(folder+"/quiz_history.csv",std::ios::binary|std::ios::trunc);if(!qf)return false;for(auto& r:records)qf<<r.sid<<','<<(long long)r.when<<','<<r.mode<<','<<r.word<<','<<(r.correct?1:0)<<'\n';qf.close();return bool(qf);}
