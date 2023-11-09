#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include "Avl.h"
#include "Chatgpt.cpp"
#include "ExtractorPalabras.h"
#include "nodoArbol.h"
#include <map>

namespace fs = std::filesystem;
using namespace std;

class ProcesoIndex{
    private:
        

    public:

        //Si necesita algo de lo anterior esta abajo jaja
        std::map<std::string, int, std::greater<>> ranking;

        ProcesoIndex(vector<string> pfrase) {//pfrase es el vector de las palabras claves de la frase de busqueda

            //Instanciamos la clase extractor y la guardamos en el vector
            diccHashTables *extractorPalabras = new diccHashTables();

            vector<std::unordered_map<std::string, std::vector<int>>> vectorHash = extractorPalabras->getVectorHash();//Vector de hash
            vector<string> libros = extractorPalabras->getLibros();

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
                    
            }
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
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            /*string folderPath = "Biblioteca"; // Reemplaza con la ruta de tu carpeta
            vector<string> fileNames;

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
                string prompt = "Dame 5 temas relacionados a este libro: " + fileName;
                Chatgpt* chat = new Chatgpt(prompt);
                vector<string> palabras = chat->getPalabras();
                hashTableTemasLibro[fileName] = palabras;
            }

            //llamo al arbol
            BinaryTree tree;
            
            //recorrer el hashtable e ir ingresando al arbol
            for (const auto& element : hashTableTemasLibro) {

                string key = element.first;  //se saca la llave
                vector<string> vectorHash = element.second;  //se saca el vector

                for (const auto& eleVector : vectorHash)
                {
                    tree.insert2(eleVector, key);  //en este caso key es el titulo del libro y tema es cada tema que hay acerca de ese libro
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
                nodo* resultado = tree.search(palabra);
                if(resultado != nullptr)
                {
                    //Revisa si la palabra se encuentra dentro de la tabla de hash, en caso de que sí, se le
                    //aumenta al valor de apariciones al libro en 1, en caso de que no exista se crea un
                    //nuevo elemento en el hash con el título del libro como key y 1 como value
                    std::string tituloBuscado = resultado->tituloLibro;
                    if(tablaRespuestas.count(tituloBuscado) == 1) {
                        tablaRespuestas[resultado->tituloLibro]++;
                    } else 
                    {
                        tablaRespuestas[resultado->tituloLibro] = 1;
                    }
                    
                }
            }

            //Ranking ordenado
            // Crear un vector de pares clave-valor para almacenar elementos del hashtable
            std::map<std::string, int, std::greater<>> tablaRespuestasOrdenada;

            // Copiar los elementos del hashtable al vector
            for (const auto& pair : tablaRespuestas) {
                // 'pair.first' es la clave (string)
                std::string clave = pair.first;
                // 'pair.second' es el vector correspondiente a la clave
                int numero = pair.second;

                tablaRespuestas[clave] = numero;
                }

            ranking = tablaRespuestasOrdenada;
            }

        map<std::string, int> getRanking(){
            return ranking;
        }

*/
};