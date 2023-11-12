#ifndef Pagina_
#define Pagina_

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

struct paginaStruct
{
    string palabra;
    std::unordered_map<int, int> paginasApariciones;
};

#endif