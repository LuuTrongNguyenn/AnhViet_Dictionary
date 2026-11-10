#include "../include/Trie.h"
#include <emscripten/emscripten.h>
#include <string>
#include <vector>
#include <sstream>
static Trie trie;
static std::string result;
extern "C" {
EMSCRIPTEN_KEEPALIVE int lex_insert(const char* word, const char* meaning) { return trie.insert(word ? word : "", meaning ? meaning : "") ? 1 : 0; }
EMSCRIPTEN_KEEPALIVE const char* lex_lookup(const char* word) {
  std::string meaning;
  if (!trie.search(word ? word : "", meaning)) { result.clear(); return result.c_str(); }
  result = meaning;
  return result.c_str();
}
EMSCRIPTEN_KEEPALIVE const char* lex_suggest(const char* prefix, int limit) {
  auto words = trie.autocomplete(prefix ? prefix : "", limit > 0 && limit <= 30 ? limit : 10);
  result.clear();
  for (size_t i=0;i<words.size();++i) { if(i) result.push_back('\n'); result += words[i]; }
  return result.c_str();
}
}
