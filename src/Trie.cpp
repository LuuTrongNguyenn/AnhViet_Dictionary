#include "../include/Trie.h"
#include <cctype>
using namespace std;
namespace {
bool normalize(const string& word, string& out) {
    out.clear();
    if (word.empty()) return false;
    for (unsigned char ch : word) {
        if (ch >= 'A' && ch <= 'Z') ch = ch - 'A' + 'a';
        if (ch < 'a' || ch > 'z') return false;
        out += static_cast<char>(ch);
    }
    return true;
}
bool hasChildren(const TrieNode* node) {
    for (auto child : node->children) if (child) return true;
    return false;
}
}
TrieNode::TrieNode() : isEndOfWord(false), meaning("") {
    for (auto& child : children) child = nullptr;
}
Trie::Trie() : root(new TrieNode()) {}
void Trie::clear(TrieNode* node) {
    if (!node) return;
    for (auto child : node->children) clear(child);
    delete node;
}
Trie::~Trie() { clear(root); }
// DSA-7: Duplicate words are rejected; use update() to change meanings.
bool Trie::insert(const string& word, const string& meaning) {
    string key;
    if (!normalize(word, key) || meaning.empty()) return false;
    TrieNode* node = root;
    for (char ch : key) {
        int idx = ch - 'a';
        if (!node->children[idx]) node->children[idx] = new TrieNode();
        node = node->children[idx];
    }
    if (node->isEndOfWord) return false;
    node->isEndOfWord = true;
    node->meaning = meaning;
    return true;
}
// DSA-8
bool Trie::search(const string& word, string& meaning) const {
    meaning.clear();
    string key;
    if (!normalize(word, key)) return false;
    const TrieNode* node = root;
    for (char ch : key) {
        node = node->children[ch - 'a'];
        if (!node) return false;
    }
    if (!node->isEndOfWord) return false;
    meaning = node->meaning;
    return true;
}
// DSA-9
bool Trie::update(const string& word, const string& newMeaning) {
    string key;
    if (!normalize(word, key) || newMeaning.empty()) return false;
    TrieNode* node = root;
    for (char ch : key) {
        node = node->children[ch - 'a'];
        if (!node) return false;
    }
    if (!node->isEndOfWord) return false;
    node->meaning = newMeaning;
    return true;
}
// DSA-10: true means the current subtree can be pruned.
bool Trie::deleteHelper(TrieNode* node, const string& word, int depth) {
    if (depth == static_cast<int>(word.size())) {
        node->isEndOfWord = false;
        node->meaning.clear();
    } else {
        int idx = word[depth] - 'a';
        TrieNode* child = node->children[idx];
        if (deleteHelper(child, word, depth + 1)) {
            delete child;
            node->children[idx] = nullptr;
        }
    }
    return !node->isEndOfWord && !hasChildren(node);
}
bool Trie::remove(const string& word) {
    string key, unused;
    if (!normalize(word, key) || !search(key, unused)) return false;
    deleteHelper(root, key, 0);
    return true;
}
// DSA-11: alphabetical DFS, including prefix itself when it is a word.
void Trie::dfs(TrieNode* node, string& current, vector<string>& results, int limit) const {
    if (static_cast<int>(results.size()) >= limit) return;
    if (node->isEndOfWord) results.push_back(current);
    for (int i = 0; i < 26 && static_cast<int>(results.size()) < limit; ++i) {
        if (!node->children[i]) continue;
        current.push_back(static_cast<char>('a' + i));
        dfs(node->children[i], current, results, limit);
        current.pop_back();
    }
}
vector<string> Trie::autocomplete(const string& prefix, int limit) const {
    vector<string> result;
    string key;
    if (limit <= 0 || !normalize(prefix, key)) return result;
    TrieNode* node = root;
    for (char ch : key) {
        node = node->children[ch - 'a'];
        if (!node) return result;
    }
    dfs(node, key, result, limit);
    return result;
}
vector<string> Trie::getAllWords() const {
    vector<string> result;
    string current;
    dfs(root, current, result, 2147483647);
    return result;
}
