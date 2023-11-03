#ifndef avl
#define avl


#include <iostream>
#include <string>
#include "nodo.h"

class BinaryTree {
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

        x->altura = max(altura(x->left), altura(x->right) + 1);
        y->altura = max(altura(y->left), altura(y->right) + 1);

        return y;
    }

    int getBalance(nodo* nodo) {
        if (nodo == nullptr) {
            return 0;
        }
        return altura(nodo->left) - altura(nodo->right);
    }

    nodo* insert(nodo* nodo, const std::string& tema, const std::string& titulo) {
        if (nodo == nullptr) {
            return new struct nodo(tema, titulo);
        }

        if (tema < nodo->tema) {
            nodo->left = insert(nodo->left, tema, titulo);
        } else if (tema > nodo->tema) {
            nodo->right = insert(nodo->right, tema, titulo);
        }

        nodo->altura = 1 + max(altura(nodo->left), altura(nodo->right));

        int balance = getBalance(nodo);

        if (balance > 1 && tema < nodo->left->tema) {
            return rotateRight(nodo);
        }
        if (balance < -1 && tema > nodo->right->tema) {
            return rotateLeft(nodo);
        }
        if (balance > 1 && tema > nodo->left->tema) {
            nodo->left = rotateLeft(nodo->left);
            return rotateRight(nodo);
        }
        if (balance < -1 && tema < nodo->right->tema) {
            nodo->right = rotateRight(nodo->right);
            return rotateLeft(nodo);
        }

        return nodo;
    }

    void inOrderTraversal(nodo* nodo) {
        if (nodo != nullptr) {
            inOrderTraversal(nodo->left);
            std::cout << "Tema: " << nodo->tema << ", Titulo: " << nodo->tituloLibro << "\n";
            inOrderTraversal(nodo->right);
        }
    }

    nodo* buscar(nodo* nodo, const std::string& tema) {
        if (nodo == nullptr || nodo->tema == tema) {
            return nodo;
        }
        if (tema < nodo->tema) {
            return buscar(nodo->left, tema);
        } else {
            return buscar(nodo->right, tema);
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    void insert2(const std::string& tema, const std::string& titulo) {
        root = insert(root, tema, titulo);
    }

    void traverseInOrder() {
        inOrderTraversal(root);
    }

    nodo* search(const std::string& tema) {
        return buscar(root, tema);
    }
};
#endif