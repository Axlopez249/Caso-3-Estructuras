#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include "Avl.h"
#include "nodo.h"
//#include "Chatgpt.cpp"
#include "ExtractorPalabras.h"
#include "BPlusTree.h"
#include <map>
#include "paginaStruct.h"
#include "paginaInterseccion.h"

namespace fs = std::filesystem;
using namespace std;

class ProcesoIndexBusqueda{
    private:
        vector<std::unordered_map<std::string, std::vector<int>>> vectorHash; //vector de hash que tienen una palabra y las paginas y sus cantidad
        vector<string> libros; //Nombre de los libros
        vector<std::unordered_map<int, std::string>> vectorHashPaginas; //Vector de las paginas para ser metidas en el el b+
        std::unordered_map<int, int> rankingLibros; //Ranking de libros

        vector<*BPlusTree> vectorArbolesB;
        
        //Instancia de clases
        diccHashTables *extractorPalabras = new diccHashTables();//Para extraer las palabras
        AVLTree arbol; //arbol avl
    public:

        ProcesoIndex() {//pfrase es el vector de las palabras claves de la frase de busqueda

            

            vectorHash = extractorPalabras->getVectorHash();//Vector de hash
            libros = extractorPalabras->getLibros();

            //Ahora se hace uso de los objetos arboles para acomodar la informacion
            arbol = new AVLTree();  


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
            //En el mismo orden del vector de los nombres del libro
            for (const auto& hash : vectorHashPaginas) {

                //Creo un arbol por cada libro
                BPlusTree *arbolB;
                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)
                for (const auto& elementHash : hash) {
                    
                    //Datos para ingresarlos al nodo
                    int numPagina = elementHash.first;
                    String contenidoPagina = elementHash.second;
                    //Datos para ingresarlos al nodo

                    //Se agrega al arbol
                    arbolB->insert(numPagina, contenidoPagina);
                    
                }
                 //Despues de indexar todas sus palabras me encargo de meterlo al vector
                vectorArbolesB.push_back(arbolB);
            }


            //Hasta el momento todo esta en arboles
            //Ahora para asegurar que todo esta guardado bien se inicializan las otras funciones despues del almacenamiento
            searchPrincipal();
            
        }


        //----------------------------------------------------------------------------------------------------------------------------------------------

        
        //Esta funcion es para obtener los libros, sus paginas y sus cantidades de repeticiones
        std::unordered_map<int, unordered_map<int, int>> busquedaAvl(string palabraBuscada){
            
            nodo* resultado = arbol.search(palabraBuscada);

            std::unordered_map<int, unordered_map<int, int>> hashRetorno;

            if (resultado) {
                //Aqui se mete el primer nodo que contenga la palabra ya que en el for siguiente están solo los asociados por lo que no está él mismo
                hashRetorno[resultado->indice] = resultado->paginasApariciones;

                // recorrer nodos asociados para meterlos en el hash la clave sería el indice del libro y como 
                //value hay otro hashtable <int, int> que me dicen el num. pagina y la cantidad de veces que está en esa pagina
                for (const auto& nodoAsociado : resultado->nodosAsociados) {
                    hashRetorno[nodoAsociado->indice] =  nodoAsociado->paginasApariciones;
                }
            } else {
                std::cout << "Palabra no encontrada: " << palabraBuscada << std::endl;
            }
 

           //Cabe destacar que esto es solo para sacar los libros de una palabra

           return hashRetorno;

        }

        //----------------------------------------------------------------------------------------------------------------------------------------------





        //----------------------------------------------------------------------------------------------------------------------------------------------

        //Sin probar
        void searchPrincipal(vector<string> pfrase){
            //Tengo cada palabra
            //Esto es un vector
            vector<vector<int>> librosTotales;
            //Voy a crear un hashtable de las palabras claves que guarden un hashtable con los libros para poder acceder despues
            std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> infoPalabrasPaginas;
            


 

            //Se recorre el vector y se toma cada frase
            for (string element : pfrase){
                
                std::unordered_map<int, std::unordered_map<int, int>> infoPalabra = busquedaAvl(element);//Tengo la info de la palabra (libro, paginas, comparaciones)
                infoPalabrasPaginas[element] = inforPalabras;
                //Con esta
                /*vector<int> vectorPalabra;
                
                for (const auto& elementHash : infoPalabra) {
                    
                    //Datos para ingresarlos al nodo
                    int indiceLibro = elementHash.first;
                    vectorPalabra.push_back(indiceLibro);  
                }
                
                librosTotales.push_back(vectorPalabra);*/
            }


            //estando hasta aquí se puede sacar el ranking mandando esta vara std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> infoPalabrasPaginas;
            rankingLibros = construirRanking(infoPalabrasPaginas);
            //Ya tenemos el ranking de palabras con las intersecciones 
            //Vamos revisando cada uno de ellos
            std::unordered_map<int, vector<paginaStruct>> hashPaginas;

            for (const auto& elementRanking : rankingLibros) {
                vector<paginaStruct> paginas;

                for (const auto& palabraHash : infoPalabrasPaginas) {
                    for (const auto& claveLibro : palabraHash.second) {
                        int keyLibro = claveLibro.first;

                        if (keyLibro == elementRanking.first) { // Comparar con elementRanking.first
                            paginaStruct elementoStruct;
                            elementoStruct.palabra = palabraHash.first;
                            elementoStruct.hashPaginas = claveLibro.second;
                            paginas.push_back(elementoStruct);
                        }
                    }
                }

                hashPaginas[elementRanking.first] = paginas; // Usar elementRanking.first como clave
            }
            

            //Hasta este momento tengo un hash que contiene el indice del libro y las paginas intersecciones por libro 
            //tomando en cuenta todas las palabras en las que se encuentra
            std::unordered_map<int, paginaInterseccion> hashInterseccion = calcularInterseccionPaginas(hashPaginas);

            //Ahora solo falta de alguna manera llamar a la funcion de busquedaBPlus para poder extraer el contenido de las pagina y sacar los parrafos que son

        }
        //----------------------------------------------------------------------------------------------------------------------------------------------



        void busquedaBPlus(){
            //Se hace la busqueda tomando en cuenta el ranking
        }


        //----------------------------------------------------------------------------------------------------------------------------------------------
        
        //Funcion para sacar la interseccion de las paginas
        std::unordered_map<int, PaginaInterseccion> calcularInterseccionPaginas(const std::unordered_map<int, std::vector<PaginaStruct>>& hashPaginas) {
            std::unordered_map<int, PaginaInterseccion> hashInterseccion;

            // Iterar sobre cada libro
            for (const auto& libro : hashPaginas) {
                int indiceLibro = libro.first;
                const std::vector<PaginaStruct>& paginas = libro.second;

                PaginaInterseccion interseccionActual;

                // Si es el primer libro, inicializar la intersección con sus páginas y palabras
                if (hashInterseccion.empty()) {
                    for (const PaginaStruct& pagina : paginas) {
                        interseccionActual.palabras.push_back(pagina.palabra);
                        interseccionActual.numPaginasInterseccion.push_back(pagina.paginasApariciones.begin()->first);
                    }
                } else {
                    // Iterar sobre las páginas del libro actual
                    for (const PaginaStruct& pagina : paginas) {
                        // Buscar la página en la intersección actual
                        auto iter = std::find_if(interseccionActual.numPaginasInterseccion.begin(),
                                                interseccionActual.numPaginasInterseccion.end(),
                                                [&](int numPagina) {
                                                    return pagina.paginasApariciones.find(numPagina) != pagina.paginasApariciones.end();
                                                });

                        // Si la página está en la intersección, actualizar las palabras
                        if (iter != interseccionActual.numPaginasInterseccion.end()) {
                            interseccionActual.palabras.push_back(pagina.palabra);
                        }
                    }
                }

                // Actualizar el resultado en el hash final
                hashInterseccion[indiceLibro] = interseccionActual;
            }

            return hashInterseccion;
        }

        


        //----------------------------------------------------------------------------------------------------------------------------------------------

        std::unordered_map<int, int> construirRanking(std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> palabras) {
            std::unordered_map<int, int> interseccion;

            if (palabras.empty()) {
                return interseccion;  // No hay palabras para calcular intersección
            }

            // Itera sobre la primera palabra para inicializar la intersección
            const auto& primerPalabra = palabras.begin()->second;
            for (const auto& entry : primerPalabra) {
                interseccion[entry.first] = entry.second;
            }

            // Itera sobre el resto de las palabras
            for (const auto& palabra : palabras) {
                const auto& libros = palabra.second;

                for (const auto& entry : interseccion) {
                    int libroId = entry.first;

                    auto iter = libros.find(libroId);
                    if (iter != libros.end()) {
                        // Actualiza la intersección utilizando el mínimo de apariciones
                        interseccion[libroId] = std::min(iter->second, entry.second);
                    } else {
                        // Si no existe en alguna palabra, no puede formar parte de la intersección
                        interseccion[libroId] = 0;
                    }
                }
            }

            return interseccion;
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------



        //----------------------------------------------------------------------------------------------------------------------------------------------

        //Sin probar
        //Esta funcion srive para retornar un vector de int de aquellos valores que se encuentran repetidos en todos los otros vectores
        std::vector<int> encontrarValoresComunes(const std::vector<std::vector<int>>& vectores) {
            // Crea un conjunto para almacenar los valores comunes
            std::unordered_set<int> valoresComunes(vectores[0].begin(), vectores[0].end());

            // Itera sobre los vectores restantes y encuentra los valores comunes
            for (size_t i = 1; i < vectores.size(); ++i) {
                std::unordered_set<int> valoresEnEsteVector(vectores[i].begin(), vectores[i].end());

                // Crea un conjunto temporal para almacenar la intersección de los conjuntos
                std::unordered_set<int> interseccion;
                for (int valor : valoresComunes) {
                    if (valoresEnEsteVector.find(valor) != valoresEnEsteVector.end()) {
                        interseccion.insert(valor);
                    }
                }

                // Actualiza el conjunto de valores comunes con la intersección encontrada
                valoresComunes = interseccion;
            }

            // Convierte el conjunto de valores comunes a un vector para devolverlo
            return std::vector<int>(valoresComunes.begin(), valoresComunes.end());
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------


        //Sin probar
        //funcion para indexar
        //esta funcion es para extraer del vector de paginas y colocarlo en un hashtable
        std::unordered_map<int, int> crearHash(vector<int> pvector){
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

        //----------------------------------------------------------------------------------------------------------------------------------------------
};
