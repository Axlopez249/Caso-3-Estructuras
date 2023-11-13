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
    // Crear un nuevo nodo interno
    BPlusNode* newNode = new BPlusNode(false);

    // Mover las claves y los hijos a newNode
    int midIndex = (ORDER - 1) / 2;
    newNode->keys = std::vector<int>(child->keys.begin() + midIndex + 1, child->keys.end());
    newNode->children = std::vector<BPlusNode*>(child->children.begin() + midIndex + 1, child->children.end());

    // Eliminar las claves y los hijos movidos de child
    child->keys.erase(child->keys.begin() + midIndex, child->keys.end());
    child->children.erase(child->children.begin() + midIndex + 1, child->children.end());

    // Insertar la clave del medio en el nodo padre
    parent->keys.insert(parent->keys.begin() + index, child->keys.back());
    child->keys.pop_back();

    // Insertar el nuevo nodo interno en el nodo padre
    parent->children.insert(parent->children.begin() + index + 1, newNode);

    // Verificar si el nodo interno padre también necesita dividirse
    if (parent->keys.size() >= ORDER) {
        int parentMidIndex = (ORDER - 1) / 2;
        BPlusNode* newParent = new BPlusNode(false);
        newParent->keys = std::vector<int>(parent->keys.begin() + parentMidIndex + 1, parent->keys.end());
        newParent->children = std::vector<BPlusNode*>(parent->children.begin() + parentMidIndex + 1, parent->children.end());
        parent->keys.erase(parent->keys.begin() + parentMidIndex, parent->keys.end());
        parent->children.erase(parent->children.begin() + parentMidIndex + 1, parent->children.end());

        // Insertar la clave del medio del nodo interno padre en el nodo abuelo
        if (parent->next != nullptr) {
            parent->next->keys.insert(parent->next->keys.begin(), parent->keys.back());
            parent->next->children.insert(parent->next->children.begin(), newParent);
        } else {
            BPlusNode* newRoot = new BPlusNode(false);
            newRoot->keys.push_back(parent->keys.back());
            newRoot->children.push_back(parent);
            newRoot->children.push_back(newParent);
            root = newRoot;
        }

        parent->keys.pop_back();
    }
}



void BPlusTree::insert(int key, const std::string& value) {
    // Caso base: el árbol está vacío
    std::cout << "Dentro de BTree" << std::endl;
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
        currentNode = currentNode->children.at(index);
    }

    std::cout << "Dentro de BTree" << std::endl;
    // Insertar en el nodo hoja
    insertInLeaf(currentNode, key, value);

    // Revisar si el nodo hoja se desbordó
    if (currentNode->keys.size() >= ORDER) {
        // Dividir el nodo hoja
        BPlusNode* newNode = new BPlusNode(true);
        newNode->next = currentNode->next;
        currentNode->next = newNode;

        std::cout << "Dentro de BTree" << std::endl;
        newNode->keys.insert(newNode->keys.begin(), currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        newNode->values.insert(newNode->values.begin(), currentNode->values.begin() + ORDER / 2, currentNode->values.end());

        currentNode->keys.erase(currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        currentNode->values.erase(currentNode->values.begin() + ORDER / 2, currentNode->values.end());

        std::cout << "Dentro de BTree" << std::endl;
        // Insertar la clave mínima del nuevo nodo hoja en el nodo padre
        if (parent == nullptr) {
            std::cout << "Parent == nullptr" << std::endl;
            // Si el nodo hoja es la raíz, crear un nuevo nodo interno como raíz
            root = new BPlusNode(false);
            root->keys.push_back(newNode->keys.at(0));
            root->children.push_back(currentNode);
            root->children.push_back(newNode);
        } else {
            std::cout << "Else parent" << std::endl;
            // Insertar en el nodo interno padre
            index = std::lower_bound(parent->keys.begin(), parent->keys.end(), newNode->keys.at(0)) - parent->keys.begin();
            parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
            parent->children.insert(parent->children.begin() + index + 1, newNode);

            std::cout << "Dentro de BTree" << std::endl;
            // Revisar si el nodo interno se desbordó
            if (parent->keys.size() >= ORDER) {
                // Dividir el nodo interno
                std::cout << "Viene split" << std::endl;
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
        currentNode = currentNode->children.at(index);
    }

    // Buscar la clave en el nodo hoja
    auto it = std::lower_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
    int index = it - currentNode->keys.begin();

    // Devolver el valor correspondiente si se encuentra la clave
    if (index < currentNode->keys.size() && currentNode->keys.at(index) == key) {
        std::cout << "Encontró" << std::endl;   //Esta línea está para ver si se encontró, será usado en las pruebas
        return currentNode->values.at(index);
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
            std::cout << node->keys.at(i) << ": " << node->values.at(i) << " | ";
        }
        std::cout << " -> ";
        if (node->next != nullptr) {
            std::cout << node->next->keys.at(0);
        }
        std::cout << std::endl;
    } else {
        for (int i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys.at(i) << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < node->children.size(); ++i) {
            printTree(node->children.at(i), indent + 4);
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
}
*/