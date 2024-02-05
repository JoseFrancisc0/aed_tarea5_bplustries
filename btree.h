#ifndef B_TREE_H
#define B_TREE_H

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
        };

        Node* root;

        void insert(T data, Node* current);
        void erase(T key, Node* current);
        bool find(T key, Node* current);

    public:
        BTree(): root(nullptr){};

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