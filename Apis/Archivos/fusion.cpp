#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Avl.cpp"
#include "Chatgpt.cpp"
#include "nodo.h"
#include <map>
#include "nodoArbol.h"

namespace fs = std::filesystem;
using namespace std;

class diccHashTables {
    private:

        nodoArbol *nuevoNodo = new nodoArbol("", 0, 0, 0, 0);

    public:

        vector<std::unordered_map<std::string, std::vector<int>>> vectorHashTables;

        diccHashTables() {  //pfrase es el vector de las palabras claves de la frase de busqueda
            string folderPath = "Biblioteca"; // Reemplaza con la ruta de tu carpeta
            vector<string> fileNames;

            try {
                for (const auto& entry : fs::directory_iterator(folderPath)) {
                    if (entry.is_regular_file()) {
                        fileNames.push_back(entry.path());
                    }
                }
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }

            unordered_map<string, vector<string>> hashTableTemasLibro;

            for (const auto& fileName : fileNames) {
                std::ifstream archivo(fileName);

                // Verifica si el archivo se abrió correctamente
                if (!archivo.is_open()) {
                    std::cout << fileName << std::endl;
                    std::cerr << "No se pudo abrir el archivo." << std::endl;
                }

                // Hashtable para almacenar las palabras y las páginas en las que se encuentran
                std::unordered_map<std::string, std::vector<int>> hashtable;

                // Variable para contar las palabras y determinar la página
                int palabraCount = 0;
                int pagina = 1; // Comenzamos en la página 1

                // Variable para almacenar cada palabra
                std::string palabra;

                // Lee y almacena palabra por palabra
                while (archivo >> palabra) {
                    if(palabra.length() > 3) {  // Limita a que las palabras tengan 4 o más carácteres
                        // Almacena la página en el vector asociado a la palabra
                        hashtable[palabra].push_back(pagina);

                        // Incrementa el contador de palabras
                        palabraCount++;

                        // Si hemos alcanzado las 200 palabras, avanzamos a la siguiente página
                        if (palabraCount >= 200) {
                            palabraCount = 0;
                            std::cout << palabra.length() << std::endl;
                            pagina++;
                        }                        
                    }

                }

                for (const auto& par : hashtable) {
                    const std::string& clave = par.first;
                    std::vector<int> valor = par.second;
//                    std::cout << "Clave: " << clave << ", Valor: " << valor << std::endl;
                }

                vectorHashTables.push_back(hashtable);

                // Cierra el archivo
                archivo.close();

            }
/*
            for (int i = 0; i < vectorHashTables.size(); i++) {
                std::unordered_map<std::string, std::vector<int>> hashPrueba = vectorHashTables.at(i);
                for (const auto& par : hashPrueba) {
                    std::string& clave = par.first;
                    std::vector<int> valor = par.second;
                    std::cout << "Clave: " << clave << ", Valor: " << valor << std::endl;
                }
            }
*/
            

            std::string palabraBuscada = "the";
            std::cout << vectorHashTables.size() << std::endl;
            std::unordered_map<std::string, std::vector<int>> hashPrueba = vectorHashTables.at(1);
            std::unordered_map<std::string, std::vector<int>>::iterator it = hashPrueba.find(palabraBuscada);

            if (it != hashPrueba.end()) {
                std::vector<int> vectorIndices = it->second;  // it->second es el vector de índices
                for (int indice : vectorIndices) {
                    std::cout << indice << std::endl;
                }
            } else {
                std::cout << "Palabra no encontrada." << std::endl;
            }

    }
};

int main() {
    diccHashTables diccionario;

    // Resto del código

    return 0;
}