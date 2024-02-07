#ifndef B_TREE_H
#define B_TREE_H

#include <iostream>
using namespace std;

template<typename T>
class BTree{
    private:
        struct Node{
            bool leaf;
            int num_keys;
            T* keys;
            Node** children;

            Node(): leaf(true){
                keys = new T[order - 1];
                children = new Node*[order];
                for(int i = 0; i < order; i++)
                    children[i] = nullptr;
            };

            ~Node(){
                delete[] keys;
                delete[] children;
            }
        };

        Node* root;
        int order;

        void insert(T data, Node* current);

        void erase(T key, Node* current);

        bool search(T key, Node* current){
            if(current == nullptr)
                return false;
            
            int i = 0;
            while(i < current->num_keys && key > current->keys[i])
                ++i;
            
            if(i < current->num_keys && key == current->keys[i])
                return true;
            else if(current->leaf)
                return false;
            else
                return search(current->children[i], key);
        }

        void clear(Node* current){
            if(current == nullptr)
                return;
            
            if(!current->leaf)
                for(int i = 0; i < current->num_keys; i++)
                    clear(current->children[i]);
            
            delete current;
        }

    public:
        BTree(int M): root(nullptr), order(M){};

        void insert(T data){
            insert(data, root);
        }

        void erase(T key){
            erase(key, root);
        }

        bool search(T key){
            return search(root, key);
        }

        void clear(){
            clear(root);
            root = nullptr;
        }

        ~BTree(){
            clear();
        }
};

#endif //B_TREE_H