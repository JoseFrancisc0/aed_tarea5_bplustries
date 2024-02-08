#ifndef B_PLUS_TREE
#define B_PLUS_TREE

class BPlus_Tree{
    private:
        struct Node{
            // TODO: MEMBERS

            Node(){}; // TODO: Constructor
        };
        Node* root;
        int order; //Minimum degree (defines the range for number of keys)

    public:
        BPlus_Tree(int M): root(nullptr), order(M){};

        void insert(int k);
        void remove(int k);
        bool search(int k);
};

#endif //B_PLUS_TREE