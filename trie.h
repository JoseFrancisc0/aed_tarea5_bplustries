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

        void erase(string s){
            if(root == nullptr)
                return;
            
            Node* current = root;
            list<pair<char, Node*>>::iterator prevChild;
            bool foundLastNode = true;

            for(char c : s){
                foundLastNode = false;
                bool found = false;
                prevChild = current->children.end();
                for(auto it = current->children.begin(); it != current->children.end(); ++it){
                    if(it->first == c){
                        current = it->second;
                        prevChild = it;
                        found = true;
                        break;
                    }
                }
                if(!found)
                    return;
            }

            if(!current->endWord)
                return;
            
            current->endWord = false;

            list<pair<char, Node*>>::iterator childToDelete;
            while(!current->endWord && current->children.empty() && current != root){
                childToDelete = prevChild;
                prevChild--;
                delete childToDelete->second;
                current->children.erase(childToDelete);
                current = root;
            }
        }

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