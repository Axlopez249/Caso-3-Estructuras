#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include "Avl.h"
#include "Chatgpt.cpp"
#include "ExtractorPalabras.h"
#include "nodoArbol.h"
#include "BPlusTree.h"
#include <map>

namespace fs = std::filesystem;
using namespace std;

class ProcesoIndex{
    private:
        vector<std::unordered_map<std::string, std::vector<int>>> vectorHash;
        vector<string> libros;
        vector<std::unordered_map<int, std::string>> vectorHashPaginas;
        //Instanciamos la clase extractor y la guardamos en el vector
        diccHashTables *extractorPalabras = new diccHashTables();
    public:

        //Si necesita algo de lo anterior esta abajo jaja
        std::map<std::string, int, std::greater<>> ranking;

        ProcesoIndex(vector<string> pfrase) {//pfrase es el vector de las palabras claves de la frase de busqueda

            

            vectorHash = extractorPalabras->getVectorHash();//Vector de hash
            libros = extractorPalabras->getLibros();

            //Ahora se hace uso de los objetos arboles para acomodar la informacion
            AVLTree *arbol = new AVLTree();  


            //Primero recorro el vector que tenemos para poder ir ingresando los datos al arbol
            for (const auto& hash : vectorHash) {
                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)
                for (const auto& elementHash : hash) {
                    
                    //Datos para ingresarlos al nodo
                    string palabra = elementHash.first;
                    vector<int> vectorPaginas = elementHash.second;
                    unordered_map<int, int> hashInt = crearHash(vectorPaginas);
                    int indice = hash.index();
                    //Datos para ingresarlos al nodo

                    //Se agrega al arbol
                    arbol->insert(palabra, hashInt, 1, indice);
                    
                }
            }

            //Tengo este vector de hashtable de paginas
            vectorHashPaginas = extractorPalabras->getVectorHashPaginas();
            //Ahora tengo que usar los arboles b+
            
            //Se crean 30 arboles - uno para cada libro
            for (const auto& hash : vectorHashPaginas) {

                //Se crea cada arbol b+
                BPlusTree arbolB;

                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)
                for (const auto& elementHash : hash) {
                    
                    //Datos para ingresarlos al nodo
                    int numPagina = elementHash.first;
                    String contenidoPagina = elementHash.second;
                    //Datos para ingresarlos al nodo

                    //Se agrega al arbol
                    arbolB.insert(numPagina, contenidoPagina);
                    
                }
            }


            //Hasta el momento todo esta en arboles
            //Fin
        }

        

        //esta funcion es para extraer
        public unordered_map<int, int> crearHash(vector<int> pvector){
            std::unordered_map<int, int> counts;

            for (int i = 0; i < pvector.size(); ++i) {
                int value = pvector[i];
                counts[value]++;
            }

            for (const auto& entry : counts) {
                std::cout << entry.first << " aparece " << entry.second << " veces" << std::endl;
            }
            return counts;
        }
            
};
