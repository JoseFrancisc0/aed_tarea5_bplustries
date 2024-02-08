#ifndef B_PLUS_TREE
#define B_PLUS_TREE

class BPlus_Tree{
    private:
        struct Node{
            int* keys; // keys in node
            int n;  // #keys in node
            int order;
            Node** C; // children of node
            Node* nextLeaf;
            Node* prevLeaf;
            bool leaf;

            Node(bool l, int t): leaf(l), order(t){
                keys = new int[2*order - 1];
                C = new Node *[2*order];
                n = 0;
            };

            ~Node(){
                delete[] keys;
                delete[] C;
            }

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