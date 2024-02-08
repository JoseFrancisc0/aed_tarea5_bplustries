#ifndef TRIE_H
#define TRIE_H

#include <list>
#include <string>
using namespace std;

class Trie{
    private:
        struct Node{
            list<pair<char, Node*>> children;
            bool endWord;

            Node(): endWord(false){}; // Implement constructor
        };
        
        Node* root;

    public:
        Trie(): root(nullptr){};

        void insert(string s);
        void erase(string s);
        bool search(string s);
        void clear();

        ~Trie(){
            clear();
        }
};

#endif //TRIE_H