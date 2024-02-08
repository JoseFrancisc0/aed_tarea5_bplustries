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

        void insert(string s){
            if(root == nullptr)
                root = new Node();
            
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
                if(!found){
                    Node* new_node = new Node();
                    current->children.push_back(make_pair(c, new_node));
                    current = new_node;
                }
            }

            current->endWord = true;
        }

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