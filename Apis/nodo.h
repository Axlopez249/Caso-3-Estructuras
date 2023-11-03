#ifndef nodo_
#define nodo_

#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct nodo {
    string tema; // Tema del libro
    string tituloLibro; // TituloLibro
    nodo* left;
    nodo* right;
    int altura;

    nodo(const string& ptema, const string& ptitulo )
        : tema(ptema), tituloLibro(ptitulo), left(nullptr), right(nullptr), altura(1){}
};

#endif