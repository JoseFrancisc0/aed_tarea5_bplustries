#ifndef B_TREE_H
#define B_TREE_H

template<typename T>
class BTree{
    private:
        struct Node{};

        Node* root;
    public:
        BTree(): root(nullptr){};

        void insert(T data);
        void erase(T key);
        bool find(T key);
        void clear();

        ~BTree();
};

#endif //B_TREE_H