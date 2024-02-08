#ifndef B_TREE_V_H
#define B_TREE_V_H

#include <iostream>
#include <algorithm>
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

        void split_child(Node* parent, int child_index, T key){
            Node* child = parent->children[child_index];
            Node* new_child = new Node(child->leaf);

            size_t median_idx = order/2;
            if(key < child->keys[median_idx])
                median_idx--;

            // Subir la key mediana
            T mediana = child->keys[median_idx];
            parent->keys.insert(parent->keys.begin() + child_index, mediana);

            // Mover keys a nuevo hijo
            new_child->keys.assign(child->keys.begin() + median_idx+1, child->keys.end());
            child->keys.resize(median_idx);

            if(!child->leaf){   // Mover hijos al nuevo
                new_child->children.assign(child->children.begin() + median_idx+1, child->children.end());
                child->children.resize(median_idx + 1);
            }

            parent->children.insert(parent->children.begin() + child_index + 1, new_child);
        }

        void insert_non_full(Node* node, T key){
            int i = node->keys.size() - 1;

            if(node->leaf){ // Insercion en hoja
                node->keys.push_back(T());
                while(i >= 0 && key < node->keys[i]){
                    node->keys[i+1] = node->keys[i];
                    i--;
                }
                node->keys[i+1] = key;
            }
            else{   // Buscar el hijo correcto
                while(i >= 0 && key < node->keys[i])
                    i--;
                i++;

                if(node->children[i]->keys.size() == order - 1){ // Hijo lleno
                    split_child(node, i, key);
                    if(key > node->keys[i])
                        i++;
                }

                insert_non_full(node->children[i], key); // insert en hijo
            }
        }

        T predecessor(Node* node, int index){
            Node* current = node->children[index];
            while(!current->leaf)
                current = current->children[current->keys.size()];
            
            return current->keys[current->keys.size() - 1];
        }

        void borrow_key(Node* parent, size_t child_index, T key, char sibling_selector){
            Node* child = parent->children[child_index];
            Node* sibling;

            if(sibling_selector == 'L'){    // Hermano izquierdo
                sibling = parent->children[child_index - 1];

                T pred1 = parent->keys[child_index - 1]; // Primer predecesor
                T pred2 = sibling->keys.back(); // Segundo predecesor

                parent->keys[child_index - 1] = pred2; // Reemplazar en padre

                size_t i = 0;
                while(i < child->keys.size() && key != child->keys[i])
                    ++i;

                child->keys[i] = pred1; // Reemplazar en hijo
                sort(child->keys.begin(), child->keys.end());
                sibling->keys.pop_back(); // Eliminar predecesor
            }
            else if(sibling_selector == 'R'){ // Hermano derecho
                sibling = parent->children[child_index + 1];

                T suc1 = parent->keys[child_index]; // Primer sucesor
                T suc2 = sibling->keys.front(); // Segundo sucesor

                parent->keys[child_index] = suc2; // Reemplazar en padre

                size_t i = 0;
                while(i < child->keys.size() && key != child->keys[i])
                    ++i;
                
                child->keys[i] = suc1; // Reemplazar en hijo
                sort(child->keys.begin(), child->keys.end());
                sibling->keys.erase(sibling->keys.begin()); // Eliminar sucesor
            }
        }

        void merge_child(Node* parent, size_t child_index, T key){
            Node* child = parent->children[child_index];
            Node* sibling;

            if(child_index + 1 < parent->children.size())
                sibling = parent->children[child_index + 1]; // Siguiente hermano
            else
                sibling = parent->children[child_index - 1]; // Anterior hermano
            
            size_t parent_key_idx = child_index < parent->keys.size() ? child_index : child_index - 1;

            // Movemos llaves de padre a hijo
            child->keys.push_back(parent->keys[parent_key_idx]);

            // Movemos llaves de hermano a hijo
            child->keys.insert(child->keys.end(), sibling->keys.begin(), sibling->keys.end());

            // Movemos hijos de hermano a hijo
            if(!sibling->leaf)
                child->children.insert(child->children.end(), sibling->children.begin(), sibling->children.end());
            
            // Eliminamos key de padre y ajustamos hijos
            parent->keys.erase(parent->keys.begin() + parent_key_idx);

            bool factor = child_index > parent->keys.size() ? 1 : 0;
            parent->children.erase(parent->children.begin() + child_index - factor);
        
            delete sibling; // Eliminamos hermano
            sort(child->keys.begin(), child->keys.end()); // Reordenando las keys de hijo

            // Eliminamos key de hijo
            size_t i = 0;
            while(i < child->keys.size() && key > child->keys[i])
                ++i;
            child->keys.erase(child->keys.begin() + i);
        }

        void erase(Node* node, T key){
            size_t i = 0;
            while(i < node->keys.size() && key > node->keys[i])
                ++i;

            if(i < node->keys.size() && key == node->keys[i]){ // Key found in node
                if(node->leaf) // Caso 0: erase en hoja
                    node->keys.erase(node->keys.begin() + i);
                else{ // Caso 1: erase en nodo interior
                    T pred = predecessor(node, i);
                    node->keys[i] = pred;
                    erase(node->children[i], pred);
                }
            }
            else{ // Key not found in node
                if(node->leaf)  // No esta en tree
                    return;
                
                size_t child_i = i;
                size_t check = ceil(order/2) + 1; // Casting to size_t

                if(node->children[child_i]->keys.size() >= check)   // Si hijo tiene espacio para borrado
                    erase(node->children[child_i], key);
                else
                    if(child_i-1 >= 0 && node->children[child_i - 1]->keys.size() > check) // Hermano izquierdo puede prestar
                        borrow_key(node, child_i, key, 'L');
                    else if(child_i + 1 < node->children.size() && node->children[child_i + 1]->keys.size() > check) // Hermano derecho puede prestar
                        borrow_key(node, child_i, key, 'R');
                    else    // Ninguno puede prestar, toca merge
                        merge_child(node, child_i, key);
            }
        }

        bool search(Node* current, T key){
            if(current == nullptr)
                return false;
            
            // Llegar hasta el indice de la llave
            size_t i = 0;
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
            
            delete current;
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
                split_child(new_root, 0, key);
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

#endif //B_TREE_V_H