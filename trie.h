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

        bool search(string s){
            Node* current = root;
            for(char c : s){
                bool found = false;
                for(auto child : current->children){
                    if(child.first == c){
                        current = child.second;
                        found = true;
                        break;
                    }
                }
                if(!found)
                    return false;
            }

            return current != nullptr && current->endWord;
        }

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