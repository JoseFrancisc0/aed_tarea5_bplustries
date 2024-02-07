#ifndef B_TREE_H
#define B_TREE_H

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class BTree{
    private:
        struct Node{
            bool leaf;
            vector<T> keys;
            vector<Node*> children;

            Node(bool l): leaf(l){};

            ~Node() = default;
        };

        Node* root;
        int order;

        void split_child(Node* parent, int child_index){
            Node* child = parent->children[child_index];
            Node* new_child = new Node(child->leaf);

            // Subir la key mediana
            T mediana = child->keys[order/2];
            parent->keys.insert(parent->keys.begin() + child_index, mediana);

            // Mover keys a nuevo hijo
            new_child->keys.assign(child->keys.begin() + (order/2)+1, child->keys.end());
            child->keys.resize(order/2);

            if(!child->leaf){   // Mover hijos al nuevo
                new_child->children.assign(child->children.begin() + (order/2)+1, child->children.end());
                child->children.resize(order/2 + 1);
            }

            parent->children.insert(parent->children.begin() + child_index + 1, new_child);
        }

        void insert_non_full(Node* node, T key){
            int i = node->keys.size() - 1;

            if(node->leaf){ // Insercion en hoja
                node->keys.push_back(T());
                while(i >= 0 && key < node->keys[i]){
                    node.keys[i+1] = node->keys[i];
                    i--;
                }
                node->keys[i+1] = key;
            }
            else{   // Buscar el hijo correcto
                while(i >= 0 && key < node->keys[i])
                    i--;
                i++;

                if(node->children[i]->keys.size() == order - 1){ // Hijo lleno
                    split_child(node, i);
                    if(key > node->keys[i])
                        i++;
                }

                insert_non_full(node->children[i], key); // insert en hijo
            }
        }

        T predecessor(Node* node, int index){
            Node* current = node->children[i];
            while(!current->leaf)
                current = current->children[current->keys.size()];
            
            return current->keys[current->keys.size() - 1];
        }

        void merge_child(Node* parent, int child_index){
            Node* child = parent->children[child_index];
            Node* sibling = parent->children[child_index + 1];

            // Movemos llaves de padre a hijo
            child->keys.push_back(parent->keys[child_index]);

            // Movemos llaves de hermano a hijo
            child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

            // Movemos hijos de hermano a hijo
            if(!sibling->leaf)
                child->children.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());
            
            // Eliminamos key de padre y ajustamos hijos
            parent->keys.erase(parent->keys.begin() + child_index);
            parent->children.erase(parent->children.begin() + child_index + 1);
        
            delete sibling;
        }

        void erase(Node* node, T key){
            int i = 0;
            while(i < node->keys.size() && key > node->keys[i])
                ++i;

            if(i < node->keys.size() && key == node->keys[i]){ // Key found in node
                if(node->leaf) // Caso 0: erase en hoja
                    node->keys.erase(node->keys.begin() + i);
                else{ // Caso 1: erase en nodo interior
                    T pred = predecessor(node, i);
                    node.keys[i] = pred;
                    erase(node->children[i], pred);
                }
            }
            else{ // Key not found in node
                if(node->leaf)  // No esta en tree
                    return;
                
                if(node->children[i]->keys.size() >= (order+1)/2)   // Caso 2: borrow de hermano
                    erase(node->children[i], key);
                else if(i != node->keys.size() && node->children[i+1]->keys.size() >= (order+1)/2) // Caso 2
                    erase(node->children[i+1], key);
                else{   // Caso 3: toca merge
                    merge_child(node, i);
                    erase(node->children[i], key);
                }
            }
        }

        bool search(Node* current, T key){
            if(current == nullptr)
                return false;
            
            // Llegar hasta el indice de la llave
            int i = 0;
            while(i < current->keys.size() && key > current->keys[i])
                ++i;

            if(i < current->keys.size() && key == current->keys[i])
                return true; // Key found
            if(current->leaf)
                return false; // Llego a una hoja y no encontro --> no esta
            
            return search(current->children[i], key);
        }

        void clear(Node* current){
            if(current == nullptr)
                return;
            
            for(Node* child : current->children)
                clear(child);
            
            delete node;
        }

    public:
        BTree(int M): root(nullptr), order(M){};

        void insert(T key){
            if(root == nullptr){ // Creando root                    
                root = new Node(true);
                root->keys.push_back(key);
                return;
            }

            if(root->keys.size() == order - 1){ // Root lleno -> split
                Node* new_root = new Node(false);
                new_root->children.push_back(root);
                split_child(new_root, 0);
                root = new_root;
            }

            insert_non_full(root, key);
        }

        void erase(T key){
            if(root == nullptr)
                return;
            
            erase(root, key);

            if(root->keys.empty() && root->leaf){ // Si root.empty(), actualizarlo
                Node* new_root = root->children[0];
                delete root;
                root = new_root;
            }
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