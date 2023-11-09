#ifndef avl_
#define avl_

#include <iostream>
#include "nodo.h"
#include <unordered_map>
#include <string>

class AVLTree {
private:
    nodo* root;

    int altura(nodo* nodo) {
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

        y->altura = max(altura(y->left), altura(y->right)) + 1;
        x->altura = max(altura(x->left), altura(x->right)) + 1;

        return x;
    }

    nodo* rotateLeft(nodo* x) {
        nodo* y = x->right;
        nodo* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->altura = max(altura(x->left), altura(x->right)) + 1;
        y->altura = max(altura(y->left), altura(y->right)) + 1;

        return y;
    }

    int getBalance(nodo* nodo) {
        if (nodo == nullptr) {
            return 0;
        }
        return altura(nodo->left) - altura(nodo->right);
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

        nodo->altura = 1 + max(altura(nodo->left), altura(nodo->right));

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

#endif
