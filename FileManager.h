#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "Trie.h"
#include <string>
#include <cstddef>
struct LoadResult { std::size_t loaded = 0, skipped = 0; bool fileOpened = false; };
class FileManager {
public:
    static LoadResult load(const std::string& path, Trie& trie);
    static bool save(const std::string& path, const Trie& trie);
};
#endif
