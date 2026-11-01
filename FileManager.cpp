#include "../include/FileManager.h"
#include <fstream>
#include <cstdio>
#include <string>
using namespace std;
LoadResult FileManager::load(const string& path, Trie& trie) {
    LoadResult result;
    ifstream in(path, ios::binary);
    if (!in) return result;
    result.fileOpened = true;
    string line;
    bool first = true;
    while (getline(in, line)) {
        if (first && line.compare(0, 3, "\xEF\xBB\xBF") == 0) line.erase(0, 3);
        first = false;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t pos = line.find('|');
        if (pos == string::npos || pos == 0 || pos + 1 >= line.size() ||
            !trie.insert(line.substr(0, pos), line.substr(pos + 1))) ++result.skipped;
        else ++result.loaded;
    }
    return result;
}
bool FileManager::save(const string& path, const Trie& trie) {
    const string tmp = path + ".tmp";
    ofstream out(tmp, ios::binary | ios::trunc);
    if (!out) return false;
    for (const string& word : trie.getAllWords()) {
        string meaning;
        if (trie.search(word, meaning)) out << word << '|' << meaning << '\n';
    }
    out.flush();
    bool ok = out.good();
    out.close();
    if (!ok) { remove(tmp.c_str()); return false; }
    // Windows rename cannot replace an existing destination: remove first.
    // This is not crash-atomic; a production app should use platform-specific replacement.
    remove(path.c_str());
    if (rename(tmp.c_str(), path.c_str()) != 0) return false;
    return true;
}
