#include "../include/Trie.h"
#include "../include/FileManager.h"
#include <cassert>
#include <iostream>
int main() {
    Trie t; std::string meaning;
    assert(!t.insert("", "empty"));
    assert(!t.insert("bad-word", "bad"));
    assert(t.insert("Car", "xe"));
    assert(!t.insert("car", "duplicate"));
    assert(t.search("CAR", meaning) && meaning == "xe");
    assert(t.insert("card", "the"));
    assert(t.insert("cat", "meo"));
    auto matches = t.autocomplete("ca", 2);
    assert(matches.size() == 2 && matches[0] == "car" && matches[1] == "card");
    assert(t.update("car", "oto"));
    assert(t.search("car", meaning) && meaning == "oto");
    assert(t.remove("car"));
    assert(!t.search("car", meaning));
    assert(t.search("card", meaning) && meaning == "the");
    assert(!t.remove("car"));
    assert(t.autocomplete("cat").size() == 1);
    assert(t.autocomplete("", 5).empty());
    assert(FileManager::save("test_dictionary.tmp", t));
    Trie reloaded;
    auto r = FileManager::load("test_dictionary.tmp", reloaded);
    assert(r.fileOpened && r.loaded == 2 && r.skipped == 0);
    assert(reloaded.search("card", meaning) && meaning == "the");
    std::remove("test_dictionary.tmp");
    std::cout << "All tests passed!\n";
}
