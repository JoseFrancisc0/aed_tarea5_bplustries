#include <iostream>
#include "trie.h"
using namespace std;

void test_trie(){
    Trie trie;
    trie.insert("chi");
    trie.insert("chino");
    trie.insert("chip");
    trie.insert("chipi");

    cout << boolalpha << trie.search("chip") << endl;
    cout << boolalpha << trie.search("chipi") << endl;
    trie.erase("chipi");
    cout << boolalpha << trie.search("chip") << endl;
    cout << boolalpha << trie.search("chipi") << endl;
}

int main(int, char**){
    std::cout << "Hello, from TAREA_SEMANA_5!\n";
}
