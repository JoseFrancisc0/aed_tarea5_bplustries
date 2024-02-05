#ifndef B_TREE_H
#define B_TREE_H

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class BTree{
    private:
        struct Node{
            vector<T> keys;
            vector<Node*> children;
            Node* father;
            bool leaf;

            Node(): father(this), leaf(true){};

            Node* suitable_children(T key, Node* current = this){
                if(current->leaf == true)
                    return current;
                
                int i = 0;
                for(T k : keys)
                    if(k < key)
                        i++;
                    else
                        break;
                    
                suitable_children(key, current->children[i]);
            }

            Node* split(Node* current = this){
                int middle_i = current->keys.size() / 2;
                T middle = current->keys[i];

                // Si no es root
                Node* f = current->father;
                f->insert_key(middle);

                // TODO
            }

            void insert_key(T data){
                int i = 0;
                for(T k : keys)
                    if(k < key)
                        i++;
                    else
                        break;
                
                keys.insert(keys.begin() + i, data);

                if(keys.size() > order)
                    split(this);
            }
        };

        Node* root;
        int order;

        void insert(T data, Node* current){
            current = current->suitable_children(data);
            current->insert_key(data);
        }

        void erase(T key, Node* current);

        bool find(T key, Node* current){
            int i = 0;
            for(auto k : current->keys)
                if(k == key)
                    return true;
                else if(k < key)
                    i++;
                else
                    break;
            
            find(key, current->children[i]);
        }

    public:
        BTree(int M): root(nullptr), order(M){};

        void insert(T data){
            insert(data, root);
        }

        void erase(T key){
            erase(key, root);
        }

        bool find(T key){
            find(key, root);
        }

        void clear();

        ~BTree(){
            clear();
        }
};

#endif //B_TREE_H