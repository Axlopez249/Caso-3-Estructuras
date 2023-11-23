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
        return (a > b) ? a : b; //Esto es como decir un if-else en una sola linea
        //Esto lo que devuelve es el maximo, si a es mayor a b entonces devuelve a o b
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

    int getBalance(nodo* nodo) {//Obtener el balanca que es la diferencia de alturas de la parte izquierda y derecha de un nodo
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
            // Agregamos un nuevo nodo con la misma palabra a la lista de nodos asociados.
            nodo->nodosAsociados.push_back(new struct nodo(palabra, paginasApariciones, altura, indice));
        }

        nodo->altura = 1 + max(getAltura(nodo->left), getAltura(nodo->right));
        //En esata zona se da a conocer el balance del nodo para generar una rotacion si es necesaria
        //como la resta es izquierda menos derecha, entoces se toman en cuenta los negativos y el orden alfabetico de las palbras
        //Osea la palabra que se quiere ingresar
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

    // Función auxiliar para buscar nodos asociados con la misma palabra en el árbol
    nodo* buscarNodosAsociados(nodo* nodo, const std::string& palabra) {
        if (nodo == nullptr) {
            return nullptr;
        }

        // Comparamos la palabra con la palabra del nodo actual
        if (palabra == nodo->palabra) {
            // La palabra coincide, devolvemos el nodo actual
            return nodo;
        } else if (palabra < nodo->palabra) {
            // La palabra buscada es menor, buscamos en el subárbol izquierdo
            return buscarNodosAsociados(nodo->left, palabra);
        } else {
            // La palabra buscada es mayor, buscamos en el subárbol derecho
            return buscarNodosAsociados(nodo->right, palabra);
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
        // Buscamos en el árbol principal
        nodo* nodoEncontrado = buscar(root, palabra);

        // Si encontramos un nodo con la palabra, verificamos nodos asociados
        if (nodoEncontrado != nullptr) {
            nodo* nodoAsociado = buscarNodosAsociados(nodoEncontrado, palabra);
            return (nodoAsociado != nullptr) ? nodoAsociado : nodoEncontrado;
        }

        return nullptr; // La palabra no se encontró en el árbol
    }
};
/*
int main() {
    AVLTree tree;

    // Palabras repetidas
    std::unordered_map<int, int> paginasApariciones1 = {{1, 1}, {2, 2}};
    std::unordered_map<int, int> paginasApariciones2 = {{3, 1}, {4, 1}};
    std::unordered_map<int, int> paginasApariciones3 = {{5, 1}, {6, 3}};

    tree.insert("apple", paginasApariciones1, 1, 1);
    tree.insert("banana", paginasApariciones2, 1, 2);
    tree.insert("apple", paginasApariciones3, 1, 3);

    // Realizar búsquedas
    std::string palabraBuscada = "apple";
    nodo* resultado = tree.search(palabraBuscada);

    if (resultado) {
        std::cout << "Palabra encontrada: " << palabraBuscada << std::endl;
        std::cout << "Páginas de aparición:" << std::endl;

        for (const auto& entry : resultado->paginasApariciones) {
            std::cout << "Página: " << entry.first << ", Apariciones: " << entry.second << std::endl;
        }

        // Mostrar nodos asociados
        std::cout << "Nodos asociados con la misma palabra:" << std::endl;
        for (const auto& nodoAsociado : resultado->nodosAsociados) {
            std::cout << "Palabra: " << nodoAsociado->palabra << ", Índice: " << nodoAsociado->indice << std::endl;
        }
    } else {
        std::cout << "Palabra no encontrada: " << palabraBuscada << std::endl;
    }

    return 0;
}
*/
#endif
