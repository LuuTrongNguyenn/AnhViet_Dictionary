#include "../include/ConsoleUI.h"
#include "../include/FileManager.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

namespace {
using namespace std;
bool colors = false;
const char *reset="\033[0m", *teal="\033[38;5;44m", *blue="\033[38;5;75m", *green="\033[38;5;78m", *red="\033[38;5;203m", *yellow="\033[38;5;221m", *muted="\033[38;5;245m", *bold="\033[1m";
string tint(const string& s, const char* c) { return colors ? string(c)+s+reset : s; }
void screen() {
    if (colors) cout << "\033[2J\033[H";
    else cout << string(4,'\n');
}
void bar() { cout << tint("  +--------------------------------------------------------------+\n",teal); }
void title(const string& name, const string& hint="") {
    screen();
    bar();
    cout << tint("  |",teal) << tint("                  ANH - VIET DICTIONARY                     ",bold) << tint("|\n",teal);
    cout << tint("  |",teal) << "                  TU DIEN SU DUNG TRIE                    " << tint("|\n",teal);
    bar();
    cout << "\n  " << tint(name,bold) << "\n";
    if (!hint.empty()) cout << "  " << tint(hint,muted) << "\n";
    cout << "\n";
}
string input(const string& label) {
    cout << "  " << tint(" > ",teal) << label;
    string s; getline(cin,s); return s;
}
void wait() {
    if (!cin) return;
    cout << "\n  " << tint("Nhấn Enter để quay lại menu...",muted);
    string s; getline(cin,s);
}
void result(bool ok, const string& success, const string& failure) {
    cout << "\n  " << tint(ok ? "[THÀNH CÔNG] " : "[THÔNG BÁO] ",ok?green:yellow)
         << (ok?success:failure) << "\n";
}
bool persist(const Trie& trie,const string& path) {
    bool ok=FileManager::save(path,trie);
    if (!ok) result(false,"","Không lưu được dữ liệu: " + path);
    return ok;
}
void suggestions(const Trie& trie,const string& prefix) {
    auto words=trie.autocomplete(prefix,10);
    cout << "  " << tint("GỢI Ý TỪ VỰNG",blue) << "  (" << words.size() << " kết quả)\n";
    cout << "  " << tint("--------------------------------------------------------------",muted) << "\n";
    if (words.empty()) { cout << "  Không tìm thấy từ phù hợp.\n"; return; }
    for (size_t i=0;i<words.size();++i) {
        string meaning;
        trie.search(words[i],meaning);
        cout << "  " << tint(to_string(i+1)+".",teal) << " " << tint(words[i],bold)
             << "  " << tint("-",muted) << "  " << meaning << "\n";
    }
}
void menu() {
    title("MENU CHÍNH","Chọn số tương ứng để sử dụng chức năng");
    cout << "  " << tint("[1]",teal) << " Tra nghĩa từ            " << tint("[2]",teal) << " Thêm từ mới\n\n";
    cout << "  " << tint("[3]",teal) << " Xóa từ                  " << tint("[4]",teal) << " Cập nhật nghĩa\n\n";
    cout << "  " << tint("[5]",teal) << " Gợi ý theo tiền tố      " << tint("[6]",teal) << " Tìm theo từng phím\n\n";
    cout << "  " << tint("--------------------------------------------------------------",muted) << "\n";
    cout << "  " << tint("[0]",yellow) << " Lưu dữ liệu và thoát\n\n";
}
void live(const Trie& trie) {
#ifdef _WIN32
    string prefix;
    while (true) {
        title("TÌM KIẾM TRỰC TIẾP","Gõ A-Z | Backspace: xóa | Enter hoặc Esc: quay lại");
        cout << "  Từ khóa: " << tint(prefix+"_",teal) << "\n\n";
        if (prefix.empty()) cout << "  " << tint("Bắt đầu nhập để xem gợi ý...",muted) << "\n";
        else suggestions(trie,prefix);
        cout.flush();
        int key=_getch();
        if (key==13 || key==27) break;
        if (key==0 || key==224) { _getch(); continue; }
        if (key==8) { if (!prefix.empty()) prefix.pop_back(); continue; }
        if (key>='a' && key<='z') prefix+=char(key);
        if (key>='A' && key<='Z') prefix+=char(key-'A'+'a');
    }
#else
    title("TÌM KIẾM THEO TIỀN TỐ");
    string prefix=input("Nhập tiền tố: ");
    cout << "\n"; suggestions(trie,prefix); wait();
#endif
}
void init() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode=0;
    if (h!=INVALID_HANDLE_VALUE && GetConsoleMode(h,&mode)) {
        colors=SetConsoleMode(h,mode|ENABLE_VIRTUAL_TERMINAL_PROCESSING)!=0;
    }
    SetConsoleTitleW(L"Anh - Viet Dictionary | Trie");
#else
    colors=true;
#endif
}
}
void ConsoleUI::run(Trie& trie,const std::string& path) {
    init();
    while (true) {
        menu();
        string choice=input("Lựa chọn (0-6): ");
        if (!cin) { persist(trie,path); return; }
        if (choice=="0") {
            bool ok=persist(trie,path);
            title("TẠM BIỆT","Cảm ơn bạn đã sử dụng từ điển Anh - Việt!");
            result(ok,"Đã lưu dữ liệu.","Không thể lưu dữ liệu. Hãy kiểm tra đường dẫn.");
            return;
        }
        if (choice=="6") { live(trie); continue; }
        if (choice!="1" && choice!="2" && choice!="3" && choice!="4" && choice!="5") {
            result(false,"","Vui lòng chọn số từ 0 đến 6."); wait(); continue;
        }
        const string names[]={"","TRA NGHĨA","THÊM TỪ MỚI","XÓA TỪ","CẬP NHẬT NGHĨA","GỢI Ý THEO TIỀN TỐ"};
        title(names[choice[0]-'0']);
        string word=input(choice=="5"?"Nhập tiền tố: ":"Nhập từ tiếng Anh: ");
        if (!cin) { persist(trie,path); return; }
        if (choice=="1") {
            string meaning;
            bool ok=trie.search(word,meaning);
            if (ok) {
                cout << "\n  " << tint("TỪ VỰNG",blue) << " : " << tint(word,bold) << "\n";
                cout << "  " << tint("NGHĨA",blue) << "    : " << meaning << "\n";
            } else result(false,"","Không tìm thấy từ này trong từ điển.");
        } else if (choice=="5") {
            cout << "\n"; suggestions(trie,word);
        } else if (choice=="3") {
            string confirm=input("Xác nhận xóa từ? (y/N): ");
            if (confirm=="y" || confirm=="Y") {
                bool ok=trie.remove(word);
                result(ok,"Đã xóa từ.","Từ không tồn tại hoặc không hợp lệ.");
                if (ok) persist(trie,path);
            } else result(false,"","Đã hủy thao tác.");
        } else {
            string meaning=input("Nhập nghĩa tiếng Việt: ");
            if (!cin) { persist(trie,path); return; }
            bool ok=choice=="2"?trie.insert(word,meaning):trie.update(word,meaning);
            result(ok,choice=="2"?"Đã thêm từ mới.":"Đã cập nhật nghĩa.","Từ không hợp lệ, đã trùng hoặc không tồn tại.");
            if (ok) persist(trie,path);
        }
        wait();
    }
}
