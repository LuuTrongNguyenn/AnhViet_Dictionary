#include "../include/Trie.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using Clock = chrono::steady_clock;
string wordFrom(int value) {
    string result = "w";
    do { result.push_back(static_cast<char>('a' + value % 26)); value /= 26; } while (value);
    return result;
}
int main() {
    for (int n : {1000, 10000, 50000}) {
        Trie trie;
        vector<string> words;
        for (int i = 0; i < n; ++i) words.push_back(wordFrom(i));
        auto start = Clock::now();
        for (const auto& w : words) trie.insert(w, "meaning");
        auto mid = Clock::now();
        string meaning;
        for (const auto& w : words) trie.search(w, meaning);
        auto end = Clock::now();
        cout << n << "," << chrono::duration<double, milli>(mid-start).count()
             << "," << chrono::duration<double, milli>(end-mid).count() << '\n';
    }
}
