#ifndef DICCHASHTABLES_
#define DICCHASHTABLES_

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
//#include "Avl.h"
//#include "Chatgpt.cpp"
//#include "nodo.h"
#include <map>

namespace fs = std::filesystem;
using namespace std;

class diccHashTables {
    private:
        vector<std::unordered_map<std::string, std::vector<int>>> vectorHashTables;//Vector de hashtables de las palabras
        vector<std::unordered_map<int, std::string>> vectorHashPaginas;//Vector de hashtables de las paginas
        vector<string> libros;
    public:

        

        diccHashTables() {//pfrase es el vector de las palabras claves de la frase de busqueda
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

            libros = fileNames;
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
                std::unordered_map<int, std::string> paginas;

                // Variable para contar las palabras y determinar la página
                int palabraCount = 0;
                int pagina = 1; // Comenzamos en la página 1

                // Variable para almacenar cada palabra
                std::string palabra;
                std::string paginaActual;

                // Lee y almacena palabra por palabra
                while (archivo >> palabra) {
                    if(palabra.length() > 3) {  // Limita a que las palabras tengan 4 o más carácteres
                        // Almacena la página en el vector asociado a la palabra
                        hashtable[palabra].push_back(pagina);
                        paginaActual += palabra + " ";
                        // Incrementa el contador de palabras
                        palabraCount++;

                        

                        // Si hemos alcanzado las 200 palabras, avanzamos a la siguiente página
                        if (palabraCount >= 200) {
                            paginas[pagina] = paginaActual;
                            palabraCount = 0;
                            //std::cout << palabra.length() << std::endl;
                            pagina++;
                            paginaActual.clear();
                        }                        
                    }
                }

                

                vectorHashTables.push_back(hashtable);
                vectorHashPaginas.push_back(paginas);

                if (!paginaActual.empty()) {
                    paginas[pagina] = paginaActual;
                }

                // Cierra el archivo
                archivo.close();

                

            }

            //std::cout << vectorHashTables.size() << std::endl;
        }

        //Get para retornar el vector de hashtables
        vector<std::unordered_map<int, std::string>> getVectorHashPaginas(){
            return vectorHashPaginas;
        }

        //Get para retornar el vector de hashtables
        vector<std::unordered_map<std::string, std::vector<int>>> getVectorHash(){
            return vectorHashTables;
        }

        //Get para retornar el orden de los libros para utilizar el indice luego
        vector<string> getLibros(){
            return libros;
        }
};
/*
int main() {
    diccHashTables diccionario;
}
*/
#endif // DICCHASHTABLES_