#ifndef B_PLUS_TREE
#define B_PLUS_TREE

class BPlus_Tree{
    private:
        struct Node{
            int* keys; // keys in node
            int n;  // #keys in node
            int order;
            Node** C; // children of node
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

            void insert_non_full(int k);

            Node* search(int k){
                int i = 0;
                while(i < n && k >= keys[i])
                    i++;
                
                if(i < n && keys[i] == k)
                    return this;
                if(leaf)
                    return nullptr;

                return C[i]->search(k);
            }

        };

        Node* root;
        int order; //Minimum degree (defines the range for number of keys)

        Node* find_leaf(Node* node, int k){
            while(!node->leaf){
                int i = 0;
                while(i < node->n && k >= node->keys[i])
                    i++;
                
                node = node->C[i];
            }
            return node;
        }

        void splitLeaf(Node* node);

    public:
        BPlus_Tree(int M): root(nullptr), order(M){};

        void insert(int k){
            if(root == nullptr){
                root = new Node(true, order);
                root->insert_non_full(k);
            }
            else{
                Node* leaf = find_leaf(root, k);

                leaf->insert_non_full(k);
                if(leaf->n == 2*order - 1)
                    splitLeaf(leaf);
            }
        }

        void remove(int k);

        bool search(int k){
            if(root == nullptr)
                return false;
            
            return root->search(k) == nullptr ? false : true;
        }
};

#endif //B_PLUS_TREE