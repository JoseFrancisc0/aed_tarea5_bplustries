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

            Node(): endWord(false){};
        };

        Node* root;

        void clear(Node* node){
            if(node == nullptr)
                return;
            
            for(auto child : node->children){
                clear(child.second);
                delete child.second;
            }

            node->children.clear();
        }

    public:
        Trie(): root(nullptr){};

        void insert(string s);
        void erase(string s);
        bool search(string s);

        void clear(){
            clear(root);
            delete root;
            root = nullptr;
        }

        ~Trie(){
            clear();
        }
};

#endif //TRIE_H