#ifndef B_TREE_H
#define B_TREE_H

#include<iostream>
using namespace std;

// A BTree node
class BTreeNode{
    private:
        int *keys;
        int t;	 // Minimum degree (defines the range for number of keys)
        BTreeNode **C;
        int n;	 // Current number of keys
        bool leaf;

    public:
        BTreeNode(int _t, bool _leaf): t(_t), leaf(_leaf){
            keys = new int[2*t - 1];
            C = new BTreeNode *[2*t];
            n = 0;
        }

        // Traverse the node
        void traverse(){
            int i;
            for (i = 0; i < n; i++){
                if (leaf == false)
                    C[i]->traverse();
                cout << " " << keys[i];
            }

            if (leaf == false)
                C[i]->traverse();
        }

        // Search k in Node
        BTreeNode* search(int k){
            int i = 0;
            while (i < n && k > keys[i])
                i++;

            if (keys[i] == k)
                return this;
            if (leaf == true)
                return NULL;

            return C[i]->search(k);        
        }

        // Find the approximate key of k in node
        int findKey(int k){
            int idx = 0;
            while (idx < n && keys[idx] < k)
                ++idx;
            return idx;        
        }

        // Insert k in non full node
        void insertNonFull(int k){
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

                if (C[i + 1]->n == 2*t - 1){
                    splitChild(i + 1, C[i + 1]);
                    if (keys[i + 1] < k)
                        i++;
                }
                C[i + 1]->insertNonFull(k);
            }        
        }

        // Splits child C[i] of node y
        void splitChild(int i, BTreeNode *y){
            BTreeNode *z = new BTreeNode(y->t, y->leaf);
            z->n = t - 1;
            for (int j = 0; j < t - 1; j++)
                z->keys[j] = y->keys[j + t];

            if (y->leaf == false){
                for (int j = 0; j < t; j++)
                    z->C[j] = y->C[j + t];
            }

            y->n = t - 1;
            for (int j = n; j >= i + 1; j--)
                C[j + 1] = C[j];
            C[i + 1] = z;

            for (int j = n - 1; j >= i; j--)
                keys[j + 1] = keys[j];

            keys[i] = y->keys[t - 1];
            n = n + 1;        
        }

        // Wrapper of remove operations for k
        void remove(int k){
            int idx = findKey(k);

            if(idx < n && keys[idx] == k)
                if(leaf)
                    removeFromLeaf(k);  // Caso 0: borrado en hoja
                else
                    removeFromNonLeaf(k); // Caso 1: borrado en nodo interno
            else{
                if(leaf)
                    return;
                
                if(C[idx]->n < t)
                    fill(idx);  // Caso 3/4: nodo no puede borrar

                if(idx == n)
                    C[idx - 1]->remove(k);
                else
                    C[idx]->remove(k);
            }
        }

        // Remove key[idx] from leaf
        void removeFromLeaf(int idx){
            for (int i = idx + 1; i < n; ++i)
                keys[i - 1] = keys[i];

            n--;
            return;        
        }

        // Remove key[idx] from non leaf
        void removeFromNonLeaf(int idx){
            int k = keys[idx];

            if (C[idx]->n >= t){
                int pred = getPred(idx);
                keys[idx] = pred;
                C[idx]->remove(pred);
            }
            else if (C[idx + 1]->n >= t){
                int succ = getSucc(idx);
                keys[idx] = succ;
                C[idx+1]->remove(succ);
            }
            else{
                merge(idx);
                C[idx]->remove(k);
            }

            return;        
        }

        // Gets predecesor of key[idx]
        int getPred(int idx){
            BTreeNode *cur = C[idx];
            while (!cur->leaf)
                cur = cur->C[cur->n];

            return cur->keys[cur->n - 1];        
        }

        // Gets succesor of key[idx]
        int getSucc(int idx){
            BTreeNode *cur = C[idx + 1];
            while (!cur->leaf)
                cur = cur->C[0];

            return cur->keys[0];
        }

        // Fill the child C[idx]
        void fill(int idx){
            if (idx != 0 && C[idx - 1]->n >= t)
                borrowFromPrev(idx);
            else if (idx != n && C[idx + 1]->n >= t)
                borrowFromNext(idx);
            else
                if (idx != n)
                    merge(idx);
                else
                    merge(idx - 1);

            return;        
        }

        // Borrow a key from the previous sibling of the child in C[idx]
        void borrowFromPrev(int idx){
            BTreeNode *child = C[idx];
            BTreeNode *sibling = C[idx-1];

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

        // Borrow a key from the next sibling of the child in C[idx]
        void borrowFromNext(int idx){
            BTreeNode *child = C[idx];
            BTreeNode *sibling = C[idx - 1];

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

        // Merge child in C[idx] with its next sibling
        void merge(int idx){
            BTreeNode *child = C[idx];
            BTreeNode *sibling = C[idx + 1];

            child->keys[t - 1] = keys[idx];
            for (int i = 0; i < sibling->n; ++i)
                child->keys[i + t] = sibling->keys[i];

            if (!child->leaf)
                for(int i = 0; i <= sibling->n; ++i)
                    child->C[i + t] = sibling->C[i];

            for (int i = idx + 1; i < n; ++i)
                keys[i - 1] = keys[i];

            for (int i = idx + 2; i <= n; ++i)
                C[i - 1] = C[i];

            child->n += sibling->n + 1;
            n--;

            delete(sibling);
            return;        
        }

        friend class BTree;
};

class BTree{
    private:
        BTreeNode *root;
        int t; // Minimum degree

    public:
        BTree(int _t): t(_t){
            root = NULL;
        }

        void traverse(){
            if (root != NULL) 
                root->traverse();
        }

        BTreeNode* search(int k){
            return (root == NULL)? NULL : root->search(k);
        }

        void insert(int k){
            if(root == NULL){
                root = new BTreeNode(t, true);
                root->keys[0] = k;
                root->n = 1;
            }
            else{
                if(root->n == 2*t - 1){
                    BTreeNode* nodo = new BTreeNode(t, false);
                    nodo->C[0] = root;
                    nodo->splitChild(0, root);

                    int i = 0;
                    if(nodo->keys[0] < k)
                        i++;    
                    nodo->C[i]->insertNonFull(k);

                    root = nodo;
                }
                else
                    root->insertNonFull(k);
            }

        }

        void remove(int k){
            if(root == NULL)
                return;
            
            root->remove(k);

            if(root->n == 0 && !root->leaf){
                BTreeNode* temp = root;
                root = root->C[0];
                delete temp;
            }
        }
};

#endif //B_TREE_H