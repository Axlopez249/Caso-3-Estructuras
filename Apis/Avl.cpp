#ifndef avl
#define avl



#include <iostream>
#include <vector>
#include <algorithm>

#include "nodo.h"

using namespace std;

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

    nodo* insert(nodo* nodo, const string& tema, const string& titulo) {
        if (nodo == nullptr) {
            return new struct nodo(tema, titulo);
        }

        if (tema < nodo->bookTitle) {
            nodo->left = insert(nodo->left, tema, titulo);
        } else if (tema > nodo->bookTitle) {
            nodo->right = insert(nodo->right, tema, titulo);
        }

        nodo->altura = 1 + max(altura(nodo->left), altura(nodo->right));

        int balance = getBalance(nodo);

        if (balance > 1 && tema < nodo->left->bookTitle) {
            return rotateRight(nodo);
        }
        if (balance < -1 && tema > nodo->right->bookTitle) {
            return rotateLeft(nodo);
        }
        if (balance > 1 && tema > nodo->left->bookTitle) {
            nodo->left = rotateLeft(nodo->left);
            return rotateRight(nodo);
        }
        if (balance < -1 && tema < nodo->right->bookTitle) {
            nodo->right = rotateRight(nodo->right);
            return rotateLeft(nodo);
        }

        return nodo;
    }

    void inOrderTraversal(nodo* nodo) {
        if (nodo != nullptr) {
            inOrderTraversal(nodo->left);
            cout << "Titulo: " << nodo->bookTitle << "\n";
            for (const string& tema : nodo->bookThemes) {
                cout << "Tema: " << tema << "\n";
            }
            inOrderTraversal(nodo->right);
        }
    }

    nodo* buscar(nodo* nodo, const string& tema) {
        if (nodo == nullptr || nodo->bookTitle == tema) {
            return nodo;
        }
        if (tema < nodo->bookTitle) {
            return buscar(nodo->left, tema);
        } else {
            return buscar(nodo->right, tema);
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    void insert(const string& tema, const string& titulo) {
        root = insert(root, tema, titulo);
    }

    void traverseInOrder() {
        inOrderTraversal(root);
    }

    nodo* search(const string& tema) {
        return buscar(root, tema);
    }
};
#endif
