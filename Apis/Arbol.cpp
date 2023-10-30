#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "ApiServer.cpp"
#include <unordered_map>
#include "PalabraLibro.h"
#include "TemaLibro.h"
#include "APISynonyms.cpp"

namespace fs = std::filesystem;

int main() {
    string folderPath = "Biblioteca"; // Reemplaza con la ruta de tu carpeta
    vector<string> fileNames;
    vector<Libro> libros;

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                fileNames.push_back(entry.path().filename());
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    unordered_map<string, vector<string>> hashTableTemasLibro;

    for (const auto& fileName : fileNames) {
        //ya tengo los titulos de los libros
        PalabrasClaves extractorPalabrasClaves = new PalabrasClaves(fileName);
        vector<string> palabrasLibro = extractorPalabrasClaves.getPalabras();
        hashTableTemasLibro[fileName] = palabraLibro;

    }

    vector<TemaLibro> temasLibros;

    for (const auto& pair : hashTablePalabrasLibro) {
        // 'pair.first' es la clave (string)
        string clave = pair.first;

        // 'pair.second' es el vector correspondiente a la clave
        vector<string> palabrasClave = pair.second;

        for (const auto& palabra : palabrasClave) {
            Synonyms extractorSinonimos(palabra);
            extractorSinonimos.extraccion();
            vector<string> sinonimospalabra = extractorSinonimos.getSinonimos();
            TemaLibro sinonimo = new TemaLibro(sinonimospalabra,clave);
            temasLibros.push_back(sinonimo);
        }
        
    }




    
    return 0;
}
