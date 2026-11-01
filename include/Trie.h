#ifndef TRIE_H
#define TRIE_H
#include <string>
#include <vector>
struct TrieNode {
    TrieNode* children[26];
    bool isEndOfWord;
    std::string meaning;
    TrieNode();
};
class Trie {
private:
    TrieNode* root;
    void clear(TrieNode* node);
    bool deleteHelper(TrieNode* node, const std::string& word, int depth);
    void dfs(TrieNode* node, std::string& current, std::vector<std::string>& results, int limit) const;
public:
    Trie();
    ~Trie();
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    bool insert(const std::string& word, const std::string& meaning);
    bool search(const std::string& word, std::string& meaning) const;
    bool update(const std::string& word, const std::string& newMeaning);
    bool remove(const std::string& word);
    std::vector<std::string> autocomplete(const std::string& prefix, int limit = 10) const;
    std::vector<std::string> getAllWords() const;
};
#endif
