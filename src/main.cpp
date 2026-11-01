#include "../include/Trie.h"
#include "../include/FileManager.h"
#include "../include/ConsoleUI.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    const std::string path = argc > 1 ? argv[1] : "data/dictionary.txt";
    Trie trie;
    LoadResult r = FileManager::load(path, trie);
    ConsoleUI::run(trie, path);
}
