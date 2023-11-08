#ifndef nodoArbol_
#define nodoArbol_

#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//Hacerlo generic

struct nodoArbol {
    string palabra;
    std::unordered_map<int, int> paginasApariciones;
    int indice;
    nodo* left;
    nodo* right;
    int altura;

    nodoArbol(const string& pPalabra, const int& pPagina, const int& pAparaciones, const int& pAltura, const int& pIndice)
        : palabra(pPalabra), paginasApariciones({{pPagina, pAparaciones}}), left(nullptr), right(nullptr), altura(1){}
};

#endif