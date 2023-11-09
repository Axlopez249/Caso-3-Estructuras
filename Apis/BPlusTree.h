#include <iostream>
#include <vector>
#include <algorithm>

const int ORDER = 3; // Orden del árbol B+

struct BPlusNode {
    std::vector<int> keys;
    std::vector<std::string> values;
    std::vector<BPlusNode*> children;
    BPlusNode* next; // Puntero al siguiente nodo hoja
    bool isLeaf;

    BPlusNode(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
};

class BPlusTree {
private:
    BPlusNode* root;

    // Función auxiliar para insertar en un nodo hoja
    void insertInLeaf(BPlusNode* node, int key, const std::string& value);

    // Función auxiliar para dividir un nodo interno
    void splitInternalNode(BPlusNode* parent, int index, BPlusNode* child);

public:
    BPlusTree() : root(nullptr) {}

    // Función para insertar una clave-valor en el árbol B+
    void insert(int key, const std::string& value);

    void printTree() {
        printTree(root, 0);
    }

    // Función para buscar un valor dado una clave en el árbol B+
    std::string search(int key);

    // Función para imprimir el árbol B+
    void printTree(BPlusNode* node, int indent = 0);
};

void BPlusTree::insertInLeaf(BPlusNode* node, int key, const std::string& value) {
    // Encontrar la posición adecuada para insertar la clave
    auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
    int index = it - node->keys.begin();

    // Insertar la clave y el valor en las posiciones correspondientes
    node->keys.insert(it, key);
    node->values.insert(node->values.begin() + index, value);
}

void BPlusTree::splitInternalNode(BPlusNode* parent, int index, BPlusNode* child) {
    // Crear un nuevo nodo interno y mover las claves y los hijos apropiados
    BPlusNode* newNode = new BPlusNode(false);
    newNode->children.insert(newNode->children.begin(), child->children.begin() + ORDER / 2, child->children.end());
    child->children.erase(child->children.begin() + ORDER / 2, child->children.end());
    newNode->keys.insert(newNode->keys.begin(), child->keys.begin() + ORDER / 2, child->keys.end());
    child->keys.erase(child->keys.begin() + ORDER / 2, child->keys.end());

    // Insertar el valor medio en el nodo padre
    parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
    parent->children.insert(parent->children.begin() + index + 1, newNode);

    // Actualizar las claves en el nodo hijo
    newNode->keys.erase(newNode->keys.begin());
}

void BPlusTree::insert(int key, const std::string& value) {
    // Caso base: el árbol está vacío
    if (root == nullptr) {
        root = new BPlusNode(true);
        root->keys.push_back(key);
        root->values.push_back(value);
        return;
    }

    BPlusNode* currentNode = root;
    BPlusNode* parent = nullptr;
    int index = 0;

    // Descender al nodo hoja adecuado
    while (!currentNode->isLeaf) {
        parent = currentNode;
        auto it = std::upper_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
        index = it - currentNode->keys.begin();
        currentNode = currentNode->children[index];
    }

    // Insertar en el nodo hoja
    insertInLeaf(currentNode, key, value);

    // Revisar si el nodo hoja se desbordó
    if (currentNode->keys.size() >= ORDER) {
        // Dividir el nodo hoja
        BPlusNode* newNode = new BPlusNode(true);
        newNode->next = currentNode->next;
        currentNode->next = newNode;

        newNode->keys.insert(newNode->keys.begin(), currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        newNode->values.insert(newNode->values.begin(), currentNode->values.begin() + ORDER / 2, currentNode->values.end());

        currentNode->keys.erase(currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        currentNode->values.erase(currentNode->values.begin() + ORDER / 2, currentNode->values.end());

        // Insertar la clave mínima del nuevo nodo hoja en el nodo padre
        if (parent == nullptr) {
            // Si el nodo hoja es la raíz, crear un nuevo nodo interno como raíz
            root = new BPlusNode(false);
            root->keys.push_back(newNode->keys[0]);
            root->children.push_back(currentNode);
            root->children.push_back(newNode);
        } else {
            // Insertar en el nodo interno padre
            index = std::lower_bound(parent->keys.begin(), parent->keys.end(), newNode->keys[0]) - parent->keys.begin();
            parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
            parent->children.insert(parent->children.begin() + index + 1, newNode);

            // Revisar si el nodo interno se desbordó
            if (parent->keys.size() >= ORDER) {
                // Dividir el nodo interno
                splitInternalNode(parent, index, newNode);
            }
        }
    }
}

std::string BPlusTree::search(int key) {
    BPlusNode* currentNode = root;

    // Descender al nodo hoja adecuado
    while (!currentNode->isLeaf) {
        auto it = std::upper_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
        int index = it - currentNode->keys.begin();
        currentNode = currentNode->children[index];
    }

    // Buscar la clave en el nodo hoja
    auto it = std::lower_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
    int index = it - currentNode->keys.begin();

    // Devolver el valor correspondiente si se encuentra la clave
    if (index < currentNode->keys.size() && currentNode->keys[index] == key) {
        return currentNode->values[index];
    }

    // Devolver una cadena vacía si la clave no se encuentra
    return "";
}

void BPlusTree::printTree(BPlusNode* node, int indent) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf) {
        for (int i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys[i] << ": " << node->values[i] << " | ";
        }
        std::cout << " -> ";
        if (node->next != nullptr) {
            std::cout << node->next->keys[0];
        }
        std::cout << std::endl;
    } else {
        for (int i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys[i] << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < node->children.size(); ++i) {
            printTree(node->children[i], indent + 4);
        }
    }
};

/*
int main() {
    BPlusTree tree;
    tree.insert(5, "Página 5");
    tree.insert(8, "Página 8");
    tree.insert(3, "Página 3");
    tree.insert(12, "Página 12");
    tree.insert(7, "Página 7");
    tree.insert(1, "Página 1");

    tree.printTree();  // Utiliza la función pública

    std::cout << "Buscando la página con clave 5: " << tree.search(5) << std::endl;
    std::cout << "Buscando la página con clave 10: " << tree.search(10) << std::endl;

    return 0;
}*/