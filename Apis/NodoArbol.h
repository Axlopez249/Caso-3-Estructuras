#ifndef nodo_
#define nodo_

#include <set>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct nodo {
    string tema; // Título del libro
    string tituloLibro; // Temas asociados al libro
    nodo* left;
    nodo* right;
    int altura;

    nodo(const string& titulo, const vector<string>& temas)
        : bookTitle(titulo), bookThemes(temas), left(nullptr), right(nullptr), altura(1){}
};

#endif