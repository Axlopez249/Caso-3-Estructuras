#ifndef avl_
#define avl_

#include <iostream>
#include "nodo.h"
#include <unordered_map>
#include <string>

class AVLTree {
private:
    nodo* root;

    int getAltura(nodo* nodo) {
        if (nodo == nullptr) {
            return 0;
        }
        return nodo->altura;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    nodo* rotateRight(nodo* y) {
        nodo* x = y->left;
        nodo* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->altura = max(getAltura(y->left), getAltura(y->right)) + 1;
        x->altura = max(getAltura(x->left), getAltura(x->right)) + 1;

        return x;
    }

    nodo* rotateLeft(nodo* x) {
        nodo* y = x->right;
        nodo* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->altura = max(getAltura(x->left), getAltura(x->right)) + 1;
        y->altura = max(getAltura(y->left), getAltura(y->right)) + 1;

        return y;
    }

    int getBalance(nodo* nodo) {
        if (nodo == nullptr) {
            return 0;
        }
        return getAltura(nodo->left) - getAltura(nodo->right);
    }

    nodo* insert(nodo* nodo, const std::string& palabra, const std::unordered_map<int, int>& paginasApariciones, int altura, int indice) {
        if (nodo == nullptr) {
            return new struct nodo(palabra, paginasApariciones, altura, indice);
        }

        if (palabra < nodo->palabra) {
            nodo->left = insert(nodo->left, palabra, paginasApariciones, altura, indice);
        } else if (palabra > nodo->palabra) {
            nodo->right = insert(nodo->right, palabra, paginasApariciones, altura, indice);
        } else {
            // La palabra ya existe en el árbol.
            // Actualizamos el número de páginas de aparición.
            for (const auto& entry : paginasApariciones) {
                nodo->paginasApariciones[entry.first] += entry.second;
            }
        }

        nodo->altura = 1 + max(getAltura(nodo->left), getAltura(nodo->right));

        int balance = getBalance(nodo);

        if (balance > 1 && palabra < nodo->left->palabra) {
            return rotateRight(nodo);
        }
        if (balance < -1 && palabra > nodo->right->palabra) {
            return rotateLeft(nodo);
        }
        if (balance > 1 && palabra > nodo->left->palabra) {
            nodo->left = rotateLeft(nodo->left);
            return rotateRight(nodo);
        }
        if (balance < -1 && palabra < nodo->right->palabra) {
            nodo->right = rotateRight(nodo->right);
            return rotateLeft(nodo);
        }

        return nodo;
    }

    void inOrderTraversal(nodo* nodo) {
        if (nodo != nullptr) {
            inOrderTraversal(nodo->left);
            std::cout << "Palabra: " << nodo->palabra << ", Índice: " << nodo->indice << "\n";
            inOrderTraversal(nodo->right);
        }
    }

    nodo* buscar(nodo* nodo, const std::string& palabra) {
        if (nodo == nullptr || nodo->palabra == palabra) {
            return nodo;
        }
        if (palabra < nodo->palabra) {
            return buscar(nodo->left, palabra);
        } else {
            return buscar(nodo->right, palabra);
        }
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(const std::string& palabra, const std::unordered_map<int, int>& paginasApariciones, int altura, int indice) {
        root = insert(root, palabra, paginasApariciones, altura, indice);
    }

    void traverseInOrder() {
        inOrderTraversal(root);
    }

    nodo* search(const std::string& palabra) {
        return buscar(root, palabra);
    }
};

int main() {
    AVLTree avlTree;

    // Insertar palabras y sus páginas de aparición en el árbol AVL
    std::unordered_map<int, int> paginas1 = {{1, 1}, {2, 2}, {3, 1}};
    avlTree.insert("palabra1", paginas1, 3, 1);

    std::unordered_map<int, int> paginas2 = {{2, 1}, {4, 2}, {5, 1}};
    avlTree.insert("palabra2", paginas2, 3, 2);

    std::unordered_map<int, int> paginas3 = {{1, 1}, {3, 1}, {6, 2}};
    avlTree.insert("palabra3", paginas3, 3, 3);

    // Realizar un recorrido en orden para ver las palabras en el árbol AVL
    std::cout << "Recorrido en orden del árbol AVL:" << std::endl;
    avlTree.traverseInOrder();

    // Buscar una palabra en el árbol AVL
    std::string palabraBuscada = "palabra2";
    nodo* nodoEncontrado = avlTree.search(palabraBuscada);
    if (nodoEncontrado != nullptr) {
        std::cout << "Palabra encontrada: " << palabraBuscada << ", Índice: " << nodoEncontrado->indice << std::endl;
    } else {
        std::cout << "Palabra no encontrada: " << palabraBuscada << std::endl;
    }

    return 0;
}

#endif
