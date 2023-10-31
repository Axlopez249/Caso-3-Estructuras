#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "ApiServer.cpp"
#include <unordered_map>
#include "PalabraLibro.h"
#include "TemaLibro.h"
#include "APISynonyms.cpp"
#include "avl.cpp"

namespace fs = std::filesystem;
using namespace std;

class CompararFrase{
    private:


    public:
        CompararFrase(vector<string> pfrase) {
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
                hashTableTemasLibro[fileName] = palabrasLibro;

            }

            vector<TemaLibro> temasLibros;

            for (const auto& pair : hashTablePalabrasLibro) {
                // 'pair.first' es la clave (string)
                string clave = pair.first;

                // 'pair.second' es el vector correspondiente a la clave
                vector<string> palabrasClave = pair.second;
                vector<string> sinonimos;
                for (const auto& palabra : palabrasClave) {
                    Synonyms extractorSinonimos(palabra);
                    extractorSinonimos.extraccion();
                    vector<string> sinonimospalabra = extractorSinonimos.getSinonimos();
                    TemaLibro sinonimo = new TemaLibro(sinonimospalabra,clave);
                    temasLibros.push_back(sinonimo);
                }
                
            }

            //llamo al arbol
            BinaryTree tree;
            
            //fileNames es un vector con los títulos de los libros
            for (const auto& fileName : fileNames) {
                //ya tengo los titulos de los libros
                for (TemaLibro libro : temasLibro)
                {
                    if (libro.getLibro() == fileName)
                    {
                        for (string tema: libro.getPalabras())
                        {
                            tree.insert(tema, fileName);
                        } 
                    }
                }
            }

            //Crea un hashtable donde se guarda el ranking
            std::unordered_map<std::string, int> tablaRespuestas;

            //Recorre un vector de palabras a buscar para ir revisando si hay comparaciones y poder sacar un ranking
            //basado en la cantidad de apariciones que tuvo un libro según el vector de frases dadas
            for (const auto& palabra : pfrase)
            {   
                //Se busca la palabra en el arbol para ver si existe, si no existe una de las palabras en el arbol
                //no sucede nada y el for sigue corriendo
                nodo* resultado = tree.buscar(palabra);
                if(resultado != nullptr)
                {
                    //Revisa si la palabra se encuentra dentro de la tabla de hash, en caso de que sí, se le
                    //aumenta al valor de apariciones al libro en 1, en caso de que no exista se crea un
                    //nuevo elemento en el hash con el título del libro como key y 1 como value
                    if(tablaRespuestas.find(resultado->bookTitle)) {
                        tablaRespuestas[resultado->bookTitle]++;
                    } else 
                    {
                        tablaRespuestas[resultado->bookTitle] = 1;
                    }
                    
                }
            }

            // Crear un vector de pares clave-valor para almacenar elementos del hashtable
            std::map<std::string, int, std::greater<>> tablaRespuestas;

            // Copiar los elementos del hashtable al vector
            for (const auto& pair : tablaRespuestas) {
                // 'pair.first' es la clave (string)
                std::string clave = pair.first;
                // 'pair.second' es el vector correspondiente a la clave
                int numero = pair.second;

                tablaRespuestas[clave] = numero;
            }

        }
};