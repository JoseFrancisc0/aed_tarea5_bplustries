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

            int find_key(int k){
                int idx = 0;
                while (idx < n && keys[idx] < k)
                    ++idx;
                return idx;  
            }

            int get_pred(int idx){
                Node* cur = C[idx];
                while (!cur->leaf)
                    cur = cur->C[cur->n];

                return cur->keys[cur->n - 1];        
            }

            int get_succ(int idx){
                Node* cur = C[idx + 1];
                while (!cur->leaf)
                    cur = cur->C[0];

                return cur->keys[0];
            }
            
            void borrow_from_prev(int idx){
                Node* child = C[idx];
                Node* sibling = C[idx-1];

                for (int i = child->n - 1; i >= 0; --i)
                    child->keys[i + 1] = child->keys[i];

                if (!child->leaf){
                    for(int i = child->n; i >= 0; --i)
                        child->C[i + 1] = child->C[i];
                }

                child->keys[0] = keys[idx - 1];
                if(!child->leaf)
                    child->C[0] = sibling->C[sibling->n];

                keys[idx - 1] = sibling->keys[sibling->n - 1];
                child->n += 1;
                sibling->n -= 1;

                return;
            }

            void borrow_from_next(int idx){
                Node* child = C[idx];
                Node* sibling = C[idx - 1];

                for (int i = child->n - 1; i >= 0; --i)
                    child->keys[i + 1] = child->keys[i];

                if (!child->leaf){
                    for(int i = child->n; i >= 0; --i)
                        child->C[i + 1] = child->C[i];
                }

                child->keys[0] = keys[idx - 1];
                if(!child->leaf)
                    child->C[0] = sibling->C[sibling->n];

                keys[idx - 1] = sibling->keys[sibling->n - 1];
                child->n += 1;
                sibling->n -= 1;

                return;        
            }

            void merge(int idx){
                Node* child = C[idx];
                Node* sibling = C[idx + 1];

                child->keys[order - 1] = keys[idx];
                for (int i = 0; i < sibling->n; ++i)
                    child->keys[i + order] = sibling->keys[i];

                if (!child->leaf)
                    for(int i = 0; i <= sibling->n; ++i)
                        child->C[i + order] = sibling->C[i];

                for (int i = idx + 1; i < n; ++i)
                    keys[i - 1] = keys[i];

                for (int i = idx + 2; i <= n; ++i)
                    C[i - 1] = C[i];

                child->n += sibling->n + 1;
                n--;

                delete(sibling);
                return; 
            }

            void fill(int idx){
                if (idx != 0 && C[idx - 1]->n >= order)
                    borrow_from_prev(idx);
                else if (idx != n && C[idx + 1]->n >= order)
                    borrow_from_next(idx);
                else
                    if (idx != n)
                        merge(idx);
                    else
                        merge(idx - 1);

                return;        
            }

            void remove_from_leaf(int idx){
                for (int i = idx + 1; i < n; ++i)
                    keys[i - 1] = keys[i];

                n--;
                return;     
            }

            void remove_from_non_leaf(int idx){
                int k = keys[idx];

                if (C[idx]->n >= order){
                    int pred = get_pred(idx);
                    keys[idx] = pred;
                    C[idx]->remove(pred);
                }
                else if (C[idx + 1]->n >= order){
                    int succ = get_succ(idx);
                    keys[idx] = succ;
                    C[idx+1]->remove(succ);
                }
                else{
                    merge(idx);
                    C[idx]->remove(k);
                }

                return;                 
            }

            // TODO: Acomodar para B+ Tree, borrado siempre en hoja (en nodo interno si aparece ahi)
            void remove(int k){
                int idx = find_key(k);

                if(idx < n && keys[idx] == k)
                    if(leaf)
                        remove_from_leaf(k);  // Caso 0: borrado en hoja
                    else
                        remove_from_non_leaf(k); // Caso 1: borrado en nodo interno
                else{
                    if(leaf)
                        return;
                    
                    if(C[idx]->n < order)
                        fill(idx);  // Caso 3/4: nodo no puede borrar

                    if(idx == n)
                        C[idx - 1]->remove(k);
                    else
                        C[idx]->remove(k);
                }
            }

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