#ifndef B_PLUS_TREE
#define B_PLUS_TREE

class BPlus_Tree{
    private:
        struct Node{
            int* keys; // keys in node
            int n;  // #keys in node
            int order;
            Node** C; // children of node
            Node* nextLeaf; // link leafs
            bool leaf;

            Node(bool l, int t): leaf(l), order(t), nextLeaf(nullptr){
                keys = new int[2*order - 1];
                C = new Node *[2*order];
                n = 0;
            };

            ~Node(){
                delete[] keys;
                delete[] C;
            }

            void split_child(int i, Node* y){
                Node* z = new Node(order, y->leaf);
                z->n = order - 1;
                for (int j = 0; j < order - 1; j++)
                    z->keys[j] = y->keys[j + order];

                if(y->leaf){
                    keys[i] = y->keys[y->order - 1];
                    z->nextLeaf = y->nextLeaf;
                    y->nextLeaf = z;
                }
                else{
                    for (int j = 0; j < order; j++)
                        z->C[j] = y->C[j + order];
                }

                y->n = order - 1;
                for (int j = n; j >= i + 1; j--)
                    C[j + 1] = C[j];
                C[i + 1] = z;

                for (int j = n - 1; j >= i; j--)
                    keys[j + 1] = keys[j];

                keys[i] = y->keys[order - 1];
                n = n + 1;                    
            }

            void insert_non_full(int k){
                int i = n - 1;

                if (leaf == true){
                    while (i >= 0 && keys[i] > k){
                        keys[i + 1] = keys[i];
                        i--;
                    }
                    keys[i + 1] = k;
                    n = n + 1;
                }
                else{
                    while (i >= 0 && keys[i] > k)
                        i--;

                    if (C[i + 1]->n == 2*order - 1){
                        split_child(i + 1, C[i + 1]);
                        if (keys[i + 1] < k)
                            i++;
                    }
                    C[i + 1]->insert_non_full(k);
                }  
            }

            void remove(int k);

            Node* search(int k){
                int i = 0;
                while(i < n && k > keys[i])
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

    public:
        BPlus_Tree(int M): root(nullptr), order(M){};

        void insert(int k){
            if(root == nullptr){
                root = new Node(true, order);
                root->keys[0] = k;
                root->n = 1;
            }
            else{
                if(root->n == 2*order - 1){
                    Node* nodo = new Node(order, false);
                    nodo->C[0] = root;
                    nodo->split_child(0, root);

                    int i = 0;
                    if(nodo->keys[0] < k)
                        i++;    
                    nodo->C[i]->insert_non_full(k);

                    root = nodo;
                }
                else
                    root->insert_non_full(k);
            }
        }

        void remove(int k){
            if(root == nullptr)
                return;
            
            root->remove(k);
        
            if(root->n == 0 && !root->leaf){
                Node* temp = root;
                root = root->C[0];
                delete temp;
            }
        }

        bool search(int k){
            if(root == nullptr)
                return false;
            
            return root->search(k) == nullptr ? false : true; 
        }
};

#endif //B_PLUS_TREE