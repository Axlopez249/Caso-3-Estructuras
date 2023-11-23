#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int ORDER = 3;

struct Page {
    int pageNumber;
    std::string content;

    Page(int num, const std::string& cont) : pageNumber(num), content(cont) {}
};

struct BPlusNode {
    std::vector<int> keys;
    std::vector<Page> pages;
    std::vector<BPlusNode*> children;
    BPlusNode* next;
    bool isLeaf;

    BPlusNode(bool leaf = false) : isLeaf(leaf), next(nullptr) {}
};

class BPlusTree {
private:
    BPlusNode* root;

    void insertInLeaf(BPlusNode* node, int key, const Page& page);
    void splitInternalNode(BPlusNode* parent, int index, BPlusNode* child);

public:
    BPlusTree() : root(nullptr) {}

    void insert(int key, const std::string& content);
    void printTree() const;
    std::string search(int key) const;

private:
    void printTree(BPlusNode* node, int indent) const;
};








void BPlusTree::insertInLeaf(BPlusNode* node, int key, const Page& page) {
    auto it = std::lower_bound(node->keys.begin(), node->keys.end(), key);
    int index = it - node->keys.begin();
    node->keys.insert(it, key);
    node->pages.insert(node->pages.begin() + index, page);
    
}





void BPlusTree::splitInternalNode(BPlusNode* parent, int index, BPlusNode* child) {
    // Verificar que el tamaño del nodo sea mayor o igual a ORDER
    if (child->keys.size() < ORDER) {
        // No es necesario dividir el nodo interno
        return;
    }

    BPlusNode* newNode = new BPlusNode(false);

    // Mover las claves a newNode
    newNode->keys.insert(newNode->keys.begin(), child->keys.begin() + ORDER / 2, child->keys.end());
    child->keys.erase(child->keys.begin() + ORDER / 2, child->keys.end());

    // Mover los hijos a newNode
    newNode->children.insert(newNode->children.begin(), child->children.begin() + ORDER / 2, child->children.end());
    child->children.erase(child->children.begin() + ORDER / 2, child->children.end());

    // Insertar la clave adecuada en el nodo padre
    parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);

    // Insertar el nuevo nodo interno en el nodo padre
    parent->children.insert(parent->children.begin() + index + 1, newNode);

    // Actualizar las claves en el nodo hijo
    newNode->keys.erase(newNode->keys.begin());
}






void BPlusTree::insert(int key, const std::string& content) {
    Page page(key, content);

    if (root == nullptr) {
        root = new BPlusNode(true);
        root->keys.push_back(key);
        root->pages.push_back(page);
        return;
    }

    BPlusNode* currentNode = root;
    BPlusNode* parent = nullptr;
    int index = 0;

    while (!currentNode->isLeaf) {
        parent = currentNode;
        auto it = std::upper_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
        index = it - currentNode->keys.begin();
        currentNode = currentNode->children.at(index);
    }

    insertInLeaf(currentNode, key, page);

    if (currentNode->keys.size() >= ORDER) {
    // Se verifica si el nodo actual excede o iguala el tamaño permitido

        BPlusNode* newNode = new BPlusNode(true);
        // Se crea un nuevo nodo hoja

        newNode->next = currentNode->next;
        currentNode->next = newNode;
        // Se ajustan los punteros para mantener la estructura de lista enlazada de las hojas

        newNode->keys.insert(newNode->keys.begin(), currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        newNode->pages.insert(newNode->pages.begin(), currentNode->pages.begin() + ORDER / 2, currentNode->pages.end());
        // Se mueven la mitad de las claves y páginas del nodo actual al nuevo nodo hoja

        currentNode->keys.erase(currentNode->keys.begin() + ORDER / 2, currentNode->keys.end());
        currentNode->pages.erase(currentNode->pages.begin() + ORDER / 2, currentNode->pages.end());
        // Se eliminan las claves y páginas que se movieron al nuevo nodo hoja del nodo actual

        if (parent == nullptr) {
            // Si el nodo actual es la raíz, se crea una nueva raíz

            root = new BPlusNode(false);
            root->keys.push_back(newNode->keys.at(0));
            root->children.push_back(currentNode);
            root->children.push_back(newNode);
            // Se actualiza la raíz con la nueva información

        } else {
            // Si el nodo actual no es la raíz

            index = std::lower_bound(parent->keys.begin(), parent->keys.end(), newNode->keys.at(0)) - parent->keys.begin();
            // Se encuentra la posición en la que se debería insertar la clave del nuevo nodo en el nodo padre

            parent->keys.insert(parent->keys.begin() + index, newNode->keys[0]);
            parent->children.insert(parent->children.begin() + index + 1, newNode);
            // Se inserta la clave del nuevo nodo y el nuevo nodo en el nodo padre

            if (parent->keys.size() >= ORDER) {
                // Si el nodo padre excede o iguala el tamaño permitido

                splitInternalNode(parent, index, newNode);
                // Se llama a la función de división del nodo interno
            }
        }
    }

}



std::string BPlusTree::search(int key) const {
    BPlusNode* currentNode = root;

    while (!currentNode->isLeaf) {
        auto it = std::upper_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
        int index = it - currentNode->keys.begin();
        currentNode = currentNode->children.at(index);
    }

    auto it = std::lower_bound(currentNode->keys.begin(), currentNode->keys.end(), key);
    int index = it - currentNode->keys.begin();

    if (index < currentNode->keys.size() && currentNode->keys.at(index) == key) {
        return currentNode->pages.at(index).content;
    }

    return "";
}




/*
void BPlusTree::printTree() const {
    printTree(root, 0);
}

void BPlusTree::printTree(BPlusNode* node, int indent) const {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf) {
        for (int i = 0; i < node->keys.size(); ++i) {
            std::cout << node->keys.at(i) << ": " << node->pages.at(i).content << " | ";
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
}*/
/*
int main() {
    BPlusTree tree;
    tree.insert(5, "Página 5");
    tree.insert(8, "Página 8");
    tree.insert(3, "Página 3");
    tree.insert(12, "Página 12");
    tree.insert(7, "Página 7");
    tree.insert(1, "Página 1");

    tree.printTree();

    std::cout << "Buscando la página con clave 5: " << tree.search(5) << std::endl;
    std::cout << "Buscando la página con clave 10: " << tree.search(10) << std::endl;

    return 0;
}
*/