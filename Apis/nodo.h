#ifndef nodo_
#define nodo_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct nodo {
    std::string palabra;
    std::unordered_map<int, int> paginasApariciones;
    std::vector<nodo*> nodosAsociados; // Vector de nodos asociados con la misma palabra
    nodo* left;
    nodo* right;
    int altura;
    int indice;

    nodo(const std::string& pPalabra, const std::unordered_map<int, int>& ppaginasApariciones, int pAltura, int pIndice)
        : palabra(pPalabra), paginasApariciones(ppaginasApariciones), left(nullptr), right(nullptr), altura(pAltura), indice(pIndice) {
    }
};

#endif 