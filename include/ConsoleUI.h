#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H
#include "Trie.h"
#include <string>
class ConsoleUI {
public:
    static void run(Trie& trie, const std::string& filePath);
};
#endif
