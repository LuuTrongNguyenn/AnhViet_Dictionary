#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <bcrypt.h>
#include <map>
#include <set>
#include <array>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/Trie.h"
#include "../include/FileManager.h"
#include "../include/SmartReview.h"
#pragma comment(lib,"ws2_32.lib")
static Trie trie;
static SmartReview review;
static std::map<std::string,std::string> wordTopics;
static std::map<std::string,std::string> wordParts;
static void loadParts(){std::ifstream f("data/word_parts.txt",std::ios::binary);std::string line;while(std::getline(f,line)){auto k=line.find('|');if(k!=std::string::npos)wordParts[line.substr(0,k)]=line.substr(k+1);}}
static bool saveParts(){std::ofstream f("data/word_parts.txt",std::ios::binary|std::ios::trunc);if(!f)return false;for(const auto& v:wordParts)f<<v.first<<'|'<<v.second<<'\n';return (bool)f;}
static bool validPart(const std::string& p){return p.empty()||p=="noun"||p=="verb"||p=="adjective"||p=="adverb"||p=="other";}
static void loadTopics(){std::ifstream f("data/word_topics.txt",std::ios::binary);std::string line;while(std::getline(f,line)){auto p=line.find('|');if(p!=std::string::npos)wordTopics[line.substr(0,p)]=line.substr(p+1);}}
static std::string esc(const std::string& s){std::string o="\"";char b[7];for(unsigned char c:s){if(c=='"'||c=='\\'){o+='\\';o+=c;}else if(c=='\n')o+="\\n";else if(c=='\r')o+="\\r";else if(c=='\t')o+="\\t";else if(c<32){sprintf(b,"\\u%04x",c);o+=b;}else o+=c;}return o+'"';}
static std::string decode(std::string s){std::string o;for(size_t i=0;i<s.size();i++){if(s[i]=='%'&&i+2<s.size()){char h[3]={s[i+1],s[i+2],0};char* e=nullptr;long v=strtol(h,&e,16);if(e==h+2){o+=(char)v;i+=2;continue;}}o+=s[i]=='+'?' ':s[i];}return o;}
static std::string param(const std::string& q,const std::string& key){size_t p=0;while(p<q.size()){size_t e=q.find('&',p);if(e==std::string::npos)e=q.size();size_t eq=q.find('=',p);if(eq!=std::string::npos&&eq<e&&q.substr(p,eq-p)==key)return decode(q.substr(eq+1,e-eq-1));p=e+1;}return "";}
static std::string normalize(std::string s){for(char& c:s){unsigned char x=(unsigned char)c;if(x>='A'&&x<='Z')c+=32;else if(x<'a'||x>'z')return "";}return s;}
static std::string arr(const std::vector<std::string>& a){std::string o="[";for(auto& s:a){if(o.size()>1)o+=',';o+=esc(s);}return o+"]";}
static std::string nums(const std::vector<int>& a){std::string o="[";for(int n:a){if(o.size()>1)o+=',';o+=std::to_string(n);}return o+"]";}
static std::string result(bool ok,const std::string& msg=""){return std::string("{\"ok\":")+(ok?"true":"false")+",\"message\":"+esc(msg)+"}";}

static std::map<std::string,std::string> sessions;
static std::string hex(const unsigned char* p,size_t n){static const char* d="0123456789abcdef";std::string s;for(size_t i=0;i<n;i++){s+=d[p[i]>>4];s+=d[p[i]&15];}return s;}
static bool unhex(const std::string& s,std::vector<unsigned char>& out){if(s.size()%2)return false;out.clear();for(size_t i=0;i<s.size();i+=2){int a=-1,b=-1;auto cv=[](char c)->int{if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;return -1;};a=cv(s[i]);b=cv(s[i+1]);if(a<0||b<0)return false;out.push_back((unsigned char)(a*16+b));}return true;}
static bool randomHex(size_t n,std::string& out){std::vector<unsigned char> b(n);if(!BCRYPT_SUCCESS(BCryptGenRandom(nullptr,b.data(),(ULONG)b.size(),BCRYPT_USE_SYSTEM_PREFERRED_RNG)))return false;out=hex(b.data(),b.size());return true;}
static bool derive(const std::string& password,const std::string& salt,std::string& hash){std::vector<unsigned char> sb;if(!unhex(salt,sb)||sb.size()!=16)return false;BCRYPT_ALG_HANDLE h=nullptr;if(!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&h,BCRYPT_SHA256_ALGORITHM,nullptr,BCRYPT_ALG_HANDLE_HMAC_FLAG)))return false;unsigned char out[32];NTSTATUS st=BCryptDeriveKeyPBKDF2(h,(PUCHAR)password.data(),(ULONG)password.size(),sb.data(),(ULONG)sb.size(),210000,out,sizeof(out),0);BCryptCloseAlgorithmProvider(h,0);if(!BCRYPT_SUCCESS(st))return false;hash=hex(out,sizeof(out));return true;}
static bool validUser(const std::string& u){if(u.size()<3||u.size()>32)return false;for(unsigned char c:u)if(!(isalnum(c)||c=='_'))return false;return true;}
static bool getUser(const std::string& u,std::string& salt,std::string& hash){std::ifstream f("data/accounts.db");std::string line;while(std::getline(f,line)){std::istringstream in(line);std::string name;if(std::getline(in,name,'|')&&std::getline(in,salt,'|')&&std::getline(in,hash)&&name==u)return true;}return false;}
static bool registerUser(const std::string& u,const std::string& pass,std::string& msg){if(!validUser(u)||pass.size()<8||pass.size()>128){msg="Tên tài khoản 3-32 ký tự (a-z, 0-9, _) và mật khẩu 8-128 ký tự";return false;}std::string salt,hash,old;if(getUser(u,old,hash)){msg="Tài khoản đã tồn tại";return false;}if(!randomHex(16,salt)||!derive(pass,salt,hash)){msg="Không thể tạo mật khẩu an toàn";return false;}std::ofstream f("data/accounts.db",std::ios::app);if(!f){msg="Không thể ghi data/accounts.db";return false;}f<<u<<'|'<<salt<<'|'<<hash<<'\n';if(!f){msg="Lỗi ghi dữ liệu";return false;}msg="Đăng ký thành công";return true;}
static bool checkUser(const std::string& u,const std::string& pass){std::string salt,stored,hash;if(!getUser(u,salt,stored)||!derive(pass,salt,hash)||stored.size()!=hash.size())return false;unsigned char diff=0;for(size_t i=0;i<hash.size();i++)diff|=(unsigned char)(stored[i]^hash[i]);return diff==0;}
static std::string cookieToken(const std::string& req){size_t p=req.find("\r\nCookie:");if(p==std::string::npos)p=req.find("\r\ncookie:");if(p==std::string::npos)return "";size_t e=req.find("\r\n",p+2);std::string line=req.substr(p+2,e-p-2);p=line.find("lexitrie_session=");if(p==std::string::npos)return "";p+=17;e=line.find(';',p);return line.substr(p,e==std::string::npos?std::string::npos:e-p);}
static bool loggedIn(const std::string& req){return sessions.count(cookieToken(req))!=0;}
static std::string api(const std::string& path,const std::string& q){
 std::string w=normalize(param(q,"word")),m=param(q,"meaning");
 if(path=="/api/suggest"){auto a=trie.autocomplete(normalize(param(q,"q")),12);return "{\"words\":"+arr(a)+"}";}
 if(path=="/api/all-words"){auto a=trie.getAllWords();std::string o="{\"words\":[";for(auto& x:a){std::string m;trie.search(x,m);if(o.size()>10)o+=',';auto it=wordTopics.find(x);o+="{\"word\":"+esc(x)+",\"meaning\":"+esc(m)+",\"topic\":"+esc(it==wordTopics.end()?"Từ vựng mở rộng":it->second)+"}";}return o+"]}";}
 if(path=="/api/learned-words")return "{\"words\":"+arr(review.learnedWords())+"}";
 if(path=="/api/topics"){std::set<std::string> ts;for(auto& kv:wordTopics)ts.insert(kv.second);std::string o="{\"topics\":[\"Tất cả chủ đề\"";for(auto& t:ts)o+=","+esc(t);return o+"]}";}
 if(path=="/api/lookup"){std::string meaning;bool ok=trie.search(w,meaning);return std::string("{\"found\":")+(ok?"true":"false")+",\"word\":"+esc(w)+",\"meaning\":"+esc(meaning)+",\"learning\":"+(review.contains(w)?"true":"false")+",\"pos\":"+esc(wordParts.count(w)?wordParts[w]:"")+"}";}
 if(path=="/api/insert"){std::string part=param(q,"pos");if(!validPart(part))return result(false,"Từ loại không hợp lệ");bool ok=!w.empty()&&!m.empty()&&trie.insert(w,m);if(ok){if(part.empty())wordParts.erase(w);else wordParts[w]=part;ok=FileManager::save("data/dictionary.txt",trie)&&saveParts();}return result(ok,ok?"Đã thêm từ":"Không thể thêm từ hoặc từ đã tồn tại");}
 if(path=="/api/update"){std::string part=param(q,"pos");if(!validPart(part))return result(false,"Từ loại không hợp lệ");bool ok=!w.empty()&&!m.empty()&&trie.update(w,m);if(ok){if(part.empty())wordParts.erase(w);else wordParts[w]=part;ok=FileManager::save("data/dictionary.txt",trie)&&saveParts();}return result(ok,ok?"Đã cập nhật":"Không tìm thấy từ");}
 if(path=="/api/delete"){bool ok=!w.empty()&&trie.remove(w);if(ok){review.remove(w);wordParts.erase(w);ok=FileManager::save("data/dictionary.txt",trie)&&review.save("data")&&saveParts();}return result(ok,ok?"Đã xóa từ":"Không tìm thấy từ");}
 if(path=="/api/add-review"){std::string meaning;bool ok=trie.search(w,meaning)&&review.add(w);if(ok)ok=review.save("data");return result(ok,ok?"Đã thêm vào sổ ôn tập":"Từ chưa tồn tại hoặc đã được thêm");}
 if(path=="/api/review"){bool ok=review.review(w,param(q,"remembered")=="1");if(ok)ok=review.save("data");return result(ok,ok?"Đã ghi nhận lượt ôn":"Không tìm thấy từ trong danh sách ôn");}
 if(path=="/api/due")return "{\"words\":"+arr(review.due())+"}";
 if(path=="/api/stats"){auto all=trie.getAllWords();int n=review.totalReviews();std::ostringstream o;o<<"{\"dictionary\":"<<all.size()<<",\"learned\":"<<review.totalWords()<<",\"due\":"<<review.due().size()<<",\"reviews\":"<<n<<",\"today\":"<<review.todayReviews()<<",\"correct\":"<<review.correctReviews()<<",\"accuracy\":"<<(n?100*review.correctReviews()/n:0)<<",\"wrong\":"<<review.wrongWords().size()<<",\"streak\":"<<review.streakDays()<<",\"days\":"<<nums(review.lastDays(28))<<",\"heatmap\":"<<nums(review.lastDays(84))<<"}";return o.str();}
 if(path=="/api/answer"){std::string md=param(q,"mode");long long sid=0;try{sid=std::stoll(param(q,"sid"));}catch(...){return result(false,"Thiếu mã phiên");}std::string meaning;bool ok=!w.empty()&&trie.search(w,meaning)&&review.recordAnswer(sid,md,w,param(q,"ok")=="1");if(ok)ok=review.save("data");return result(ok,ok?"Đã ghi nhận":"Không thể ghi kết quả");}
 if(path=="/api/wrong-words"){std::vector<std::pair<std::time_t,std::string>> v;for(auto& kv:review.wrongWords())v.push_back({kv.second.last,kv.first});std::sort(v.begin(),v.end(),[](const std::pair<std::time_t,std::string>& a,const std::pair<std::time_t,std::string>& b){return a.first>b.first;});std::string o="{\"words\":[";for(auto& p:v){std::string m;if(!trie.search(p.second,m))continue;auto& info=review.wrongWords().at(p.second);if(o.size()>10)o+=',';auto it=wordTopics.find(p.second);o+="{\"word\":"+esc(p.second)+",\"meaning\":"+esc(m)+",\"topic\":"+esc(it==wordTopics.end()?"Từ vựng mở rộng":it->second)+",\"count\":"+std::to_string(info.count)+",\"last\":"+std::to_string((long long)info.last)+",\"learned\":"+(review.contains(p.second)?"true":"false")+"}";}return o+"]}";}
 if(path=="/api/wrong-remove"){bool ok=review.removeWrong(w);if(ok)ok=review.save("data");return result(ok,ok?"Đã bỏ khỏi danh sách còn sai":"Từ không nằm trong danh sách còn sai");}
 if(path=="/api/history"){struct S{std::time_t when=0;std::string mode;std::vector<const QuizRecord*> items;};std::map<long long,S> ss;for(auto& r:review.quizRecords()){auto& s=ss[r.sid];if(s.items.empty()){s.when=r.when;s.mode=r.mode;}s.items.push_back(&r);}std::string o="{\"sessions\":[";int n=0;for(auto it=ss.rbegin();it!=ss.rend()&&n<100;++it,++n){auto& s=it->second;int good=0;for(auto* r:s.items)if(r->correct)good++;if(n)o+=',';o+="{\"sid\":"+std::to_string(it->first)+",\"when\":"+std::to_string((long long)s.when)+",\"mode\":"+esc(s.mode)+",\"total\":"+std::to_string(s.items.size())+",\"correct\":"+std::to_string(good)+",\"items\":[";bool first=true;for(auto* r:s.items){std::string m;trie.search(r->word,m);if(!first)o+=',';first=false;o+="{\"word\":"+esc(r->word)+",\"meaning\":"+esc(m)+",\"ok\":"+(r->correct?"true":"false")+"}";}o+="]}";}return o+"]}";}
 return "{\"error\":\"Unknown endpoint\"}";
}
static bool readFile(const std::string& path,std::string& out){std::ifstream f(path,std::ios::binary);if(!f)return false;std::ostringstream s;s<<f.rdbuf();out=s.str();return true;}
int main(){SetConsoleOutputCP(CP_UTF8);WSADATA wsa{};if(WSAStartup(MAKEWORD(2,2),&wsa))return 1;auto loaded=FileManager::load("data/dictionary.txt",trie);if(!loaded.fileOpened)std::cerr<<"Cannot open dictionary.txt\n";review.load("data");loadTopics();loadParts();SOCKET server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(server==INVALID_SOCKET)return 1; sockaddr_in addr{};addr.sin_family=AF_INET;addr.sin_port=htons(18765);inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);if(bind(server,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR){std::cerr<<"Port 18765 is busy\n";return 2;}listen(server,16);std::cout<<"LexiTrie ready: http://127.0.0.1:18765\n";
 while(true){
 SOCKET client=accept(server,nullptr,nullptr);if(client==INVALID_SOCKET)continue;
 std::string request;char buf[8192];int r=recv(client,buf,sizeof(buf),0);if(r<=0){closesocket(client);continue;}request.assign(buf,r);
 size_t headerEnd=request.find("\r\n\r\n");while(headerEnd==std::string::npos&&request.size()<65536){r=recv(client,buf,sizeof(buf),0);if(r<=0)break;request.append(buf,r);headerEnd=request.find("\r\n\r\n");}
 if(headerEnd==std::string::npos){closesocket(client);continue;}
 size_t a=request.find(' '),b=request.find(' ',a==std::string::npos?0:a+1);if(a==std::string::npos||b==std::string::npos){closesocket(client);continue;}
 std::string method=request.substr(0,a),uri=request.substr(a+1,b-a-1),path=uri.substr(0,uri.find('?')),q=uri.find('?')==std::string::npos?"":uri.substr(uri.find('?')+1);
 size_t cl=0,p=request.find("\r\nContent-Length:");if(p==std::string::npos)p=request.find("\r\ncontent-length:");if(p!=std::string::npos){p=request.find(':',p)+1;cl=(size_t)strtoul(request.c_str()+p,nullptr,10);}
 std::string body,mime="text/html; charset=utf-8",extra;int status=200;
 if(cl>4096){status=413;body="Request too large";}
 else{
  while(request.size()-headerEnd-4<cl){r=recv(client,buf,sizeof(buf),0);if(r<=0)break;request.append(buf,r);}
  std::string post=request.substr(headerEnd+4,cl);
  bool isAuth=(path=="/api/register"||path=="/api/login"||path=="/api/logout");
  if(path.rfind("/api/",0)==0){
   mime="application/json; charset=utf-8";
   if(isAuth&&method!="POST"){status=405;body=result(false,"POST required");}
   else if(isAuth){
    std::string origin;size_t op=request.find("\r\nOrigin:");if(op==std::string::npos)op=request.find("\r\norigin:");if(op!=std::string::npos){size_t st=request.find(':',op)+1,en=request.find("\r\n",st);origin=request.substr(st,en-st);origin.erase(0,origin.find_first_not_of(" \t"));}
    if(!origin.empty()&&origin!="http://127.0.0.1:18765"&&origin!="http://localhost:18765"){status=403;body=result(false,"Origin not allowed");}
    else if(path=="/api/logout"){sessions.erase(cookieToken(request));extra="Set-Cookie: lexitrie_session=; Path=/; HttpOnly; SameSite=Strict; Max-Age=0\r\n";body=result(true,"Đã đăng xuất");}
    else{std::string u=param(post,"username"),pw=param(post,"password"),msg;bool ok=false;
     if(path=="/api/register")ok=registerUser(u,pw,msg);
     else{ok=checkUser(u,pw);msg=ok?"Đăng nhập thành công":"Sai tài khoản hoặc mật khẩu";}
     if(ok){std::string token;if(randomHex(32,token)){sessions[token]=u;extra="Set-Cookie: lexitrie_session="+token+"; Path=/; HttpOnly; SameSite=Strict\r\n";}else{ok=false;msg="Không thể tạo phiên đăng nhập";}}
     body=result(ok,msg);
    }
   }
   else if(path=="/api/session")body=std::string("{\"ok\":")+(loggedIn(request)?"true":"false")+"}";
   else if(!loggedIn(request)){status=401;body=result(false,"Vui lòng đăng nhập");}
   else if(method!="GET"){status=405;body=result(false,"Method not allowed");}
   else body=api(path,q);
  }else if(method!="GET"){status=405;body="Method not allowed";}
  else if(path=="/"||path=="/index.html"){if(!readFile("web/index.html",body))status=404;}
  else if(path=="/style.css"){mime="text/css; charset=utf-8";if(!readFile("web/style.css",body))status=404;}
  else if(path=="/app.js"){mime="application/javascript; charset=utf-8";if(!readFile("web/app.js",body))status=404;}
  else if(path=="/login.js"){mime="application/javascript; charset=utf-8";if(!readFile("web/login.js",body))status=404;}
  else if(path=="/login.css"){mime="text/css; charset=utf-8";if(!readFile("web/login.css",body))status=404;}
  else status=404;
 }
 if(status==404)body="Not found";
 std::ostringstream h;h<<"HTTP/1.1 "<<status<<" "<<(status==200?"OK":status==401?"Unauthorized":status==403?"Forbidden":status==405?"Method Not Allowed":"Error")<<"\r\nContent-Type: "<<mime<<"\r\nX-Content-Type-Options: nosniff\r\nCache-Control: no-store\r\n"<<extra<<"Content-Length: "<<body.size()<<"\r\nConnection: close\r\n\r\n";
 auto response=h.str()+body;size_t sent=0;while(sent<response.size()){int n=send(client,response.data()+sent,(int)std::min<size_t>(response.size()-sent,65536),0);if(n<=0)break;sent+=n;}closesocket(client);
 }WSACleanup();}
