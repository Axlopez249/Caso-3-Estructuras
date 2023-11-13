#ifndef PROCESOINDEXBUSQUEDA_
#define PROCESOINDEXBUSQUEDA_

#include <iostream>
#include <filesystem>
#include <iterator>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Avl.cpp"
#include "nodo.h"
//#include "Chatgpt.cpp"
#include "ExtractorPalabras.cpp"
#include "BPlusTree.h"
#include <map>
#include "paginaStruct.h"
#include "fragmentoStruct.h"
//#include "paginaInterseccion.h"

namespace fs = std::filesystem;
using namespace std;

class ProcesoIndexBusqueda{
    private:
        vector<std::unordered_map<std::string, std::vector<int>>> vectorHash; //vector de hash que tienen una palabra y las paginas y sus cantidad
        vector<string> libros; //Nombre de los libros
        vector<string> frase;
        vector<std::unordered_map<int, std::string>> vectorHashPaginas; //Vector de las paginas para ser metidas en el el b+
        std::vector<std::pair<int, int>> rankingLibros; //Ranking de libros
        std::unordered_map <int, vector<fragmentoStruct>> impresionFinal;

        AVLTree *arbol;

        vector<BPlusTree*> vectorArbolesB;
        
        //Instancia de clases
        diccHashTables *extractorPalabras = new diccHashTables();//Para extraer las palabras
//        AVLTree arbol; //arbol avl
    public:

        void ProcesoIndex(vector<string> pfrase) {    //pfrase es el vector de las palabras claves de la frase de busqueda
            
            vectorHash = extractorPalabras->getVectorHash();//Vector de hash
            libros = extractorPalabras->getLibros();

            //Ahora se hace uso de los objetos arboles para acomodar la informacion
            //arbol* = new AVLTree();  
            

            arbol = new AVLTree();

            int indicePrimero = 1;


            //Primero recorro el vector que tenemos para poder ir ingresando los datos al arbol
            for (const auto& hash : vectorHash) {
                std::cout << indicePrimero << std::endl;
                indicePrimero++;
                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)
//                int indiceAdentro = 0;
                for (const auto& elementHash : hash) {

//                    std::cout << indiceAdentro << std::endl;

//                    indiceAdentro++;
                    
                    //Datos para ingresarlos al nodo
                    string palabra = elementHash.first;
                    vector<int> vectorPaginas = elementHash.second;
                    unordered_map<int, int> hashInt = crearHash(vectorPaginas);
                    //Sacar indice del elemento
                    int indice = std::distance(vectorHash.begin(), std::find(vectorHash.begin(), vectorHash.end(), hash));
                    //Datos para ingresarlos al nodo
//                    std::cout << indiceAdentro << std::endl;
                    //Se agrega al arbol
                    arbol->insert(palabra, hashInt, 1, indice);
//                    std::cout << indiceAdentro << std::endl;
                    
                }
            }
            //std::cout << "HAHA2" << std::endl;

            //Tengo este vector de hashtable de paginas
            vectorHashPaginas = extractorPalabras->getVectorHashPaginas();

            std::cout << vectorHashPaginas.size() << std::endl;
            //Ahora tengo que usar los arboles b+
            
            //Se crean 30 arboles - uno para cada libro
            //En el mismo orden del vector de los nombres del libro

//            int indiceAfuera = 0;
            //std::cout << "HAHA2" << std::endl;
            for (const auto& hash : vectorHashPaginas) {

//                std::cout << indiceAfuera << std::endl;

//                indiceAfuera++;

                //Creo un arbol por cada libro
                BPlusTree *arbolB;

                arbolB = new BPlusTree();
                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)

//                int indiceAdentro = 0;

                for (const auto& elementHash : hash) {

//                    std::cout << indiceAdentro << std::endl;

//                    indiceAdentro++;
                    
//                    Datos para ingresarlos al nodo
                    int numPagina = elementHash.first;
                    string contenidoPagina = elementHash.second;
                    //Datos para ingresarlos al nodo
//                    std::cout << "Antes de agregado" << std::endl;
//                    std::cout << numPagina << std::endl;
//                    std::cout << contenidoPagina << std::endl;
                    //Se agrega al arbol
                    arbolB->insert(numPagina, contenidoPagina);
//                    std::cout << "Después de agregado" << std::endl;
                    
                }
                std::cout << "HAHA2" << std::endl;
                 //Despues de indexar todas sus palabras me encargo de meterlo al vector
                vectorArbolesB.push_back(arbolB);
            }

            //Hasta el momento todo esta en arboles
            //Ahora para asegurar que todo esta guardado bien se inicializan las otras funciones despues del almacenamiento

            
            frase = pfrase;

            searchPrincipal(pfrase);
            
        }


        //----------------------------------------------------------------------------------------------------------------------------------------------

        
        //Esta funcion es para obtener los libros, sus paginas y sus cantidades de repeticiones
        std::unordered_map<int, unordered_map<int, int>> busquedaAvl(string palabraBuscada){
            
            nodo* resultado = arbol->search(palabraBuscada);

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
            std::cout << "Marca" << std::endl;
            vector<vector<int>> librosTotales;
            //Voy a crear un hashtable de las palabras claves que guarden un hashtable con los libros para poder acceder despues
            std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> infoPalabrasPaginas;
            


 

            //Se recorre el vector y se toma cada frase
            for (string element : pfrase){
                
                std::unordered_map<int, std::unordered_map<int, int>> infoPalabra = busquedaAvl(element);//Tengo la info de la palabra (libro, paginas, comparaciones)
                infoPalabrasPaginas[element] = infoPalabra;
            }


            //estando hasta aquí se puede sacar el ranking mandando esta vara std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> infoPalabrasPaginas;
            rankingLibros = construirRanking(infoPalabrasPaginas);
            //Ya tenemos el ranking de palabras con las intersecciones 
            //Vamos revisando cada uno de ellos
            
            std::unordered_map<int, vector<paginaStruct>> hashPaginas;

            for (const auto& elementRanking : rankingLibros) {//Cada elemento de este es el indice del libro
                vector<paginaStruct> paginas;//Se crea un vector con esa estructura
                for (const auto& palabraHash : infoPalabrasPaginas) { //Se recorre la informacion que se tenia anteriormente
                    for (const auto& claveLibro : palabraHash.second) {//Se entra al hash que tiene los libros por palabra
                        int keyLibro = claveLibro.first;//Llave del libro
                        
                        if (keyLibro == elementRanking.first) { // Comparar con elementRanking.first para obtener la informacion de ese libro
                            paginaStruct elementoStruct;
                            elementoStruct.palabra = palabraHash.first;
                            elementoStruct.paginasApariciones = claveLibro.second;
                            paginas.push_back(elementoStruct);
                        }
                    }
                }

                hashPaginas[elementRanking.first] = paginas; // Usar elementRanking.first como clave
            }
            
            std::cout << "Marca" << std::endl;
            std::unordered_map<int, vector<int>> hashPaginasImportantes = construirRankingPaginas(hashPaginas); //el hash que guarda lo de esa funcion, guarda 3 paginas por cada libro

            //De aquí para abajo estará mi lógica, este comentario lo anoto por si es necesario borrar mi lógica debido a que es incorrecta
            std::cout << "Marca2-Prueba" << std::endl;
            impresionFinal = busquedaBPlus(hashPaginasImportantes);

            for (const auto &impresion : impresionFinal)
            {
                cout << "libro: " << impresion.first << endl;
                for (const auto &contenido : impresion.second)
                {
                    cout << contenido << endl;
                }
                
            }
            

            

            //Ahora solo falta de alguna manera llamar a la funcion de busquedaBPlus para poder extraer el contenido de las pagina y sacar los parrafos que son

        }
        //----------------------------------------------------------------------------------------------------------------------------------------------

        std::unordered_map <int, vector<fragmentoStruct>> busquedaBPlus(std::unordered_map<int, vector<int>> hashPaginasImportantes){
            //Se hace la busqueda tomando en cuenta el ranking
            std::unordered_map <int, vector<fragmentoStruct>> pimpresionFinal;

            for (const auto &libro : hashPaginasImportantes){

                int idLibro = libro.first;
                vector<int> paginas = libro.second;

                BPlusTree *arbolActual = vectorArbolesB.at(idLibro);

                // Imprime el índice del libro
                //std::cout << "Libro #" << indiceLibro << ": ";
                vector<fragmentoStruct> fragmentos;
                for(int pagina : paginas) {
                    //std::cout << pagina << std::endl;
                    string contenidoParrafo = arbolActual->search(pagina);
                    //string fragmento = obtenerFragmento(contenido);
                    fragmentoStruct fragmento;
                    fragmento.contenido = contenidoParrafo;
                    fragmento.numPagina = pagina;
                    fragmentos.push_back(fragmento);
                    //Ahora tengo que descomponer la pagina
                }
                pimpresionFinal[idLibro] = fragmentos;
            }
            /*
            std::cout << "Recorriendo el unordered_map:2" << std::endl;
            for (const auto& entrada : impresionFinal) {
                int clave = entrada.first;
                const std::vector<std::string>& valores = entrada.second;

                std::cout << "Clave: " << clave << std::endl;
                    
                // Recorrer el vector asociado a cada clave
                std::cout << "Palabras asociadas:" << std::endl;
                for (const auto& palabra : valores) {
                    std::cout << palabra << std::endl;
                }

                std::cout << std::endl;
            }*/

            return pimpresionFinal;

        }


        /*
        std::string obtenerFragmento(const std::string& texto) {
            std::istringstream iss(texto);
            std::vector<std::string> palabras_texto{
                std::istream_iterator<std::string>{iss},
                std::istream_iterator<std::string>{}
            };

            int inicio = 0;
            int fin = 0;

            for (int i = 0; i < palabras_texto.size(); ++i) {
                if (palabras_texto[i] == frase[0]) {
                    // Encontramos la primera palabra en el vector
                    inicio = i;
                    fin = i;

                    for (int j = 1; j < frase.size(); ++j) {
                        if (i + j < palabras_texto.size() && palabras_texto[i + j] == frase[j]) {
                            fin = i + j;
                        } else {
                            // No encontramos una palabra, reiniciar la búsqueda desde el siguiente índice
                            inicio = 0;
                            fin = 0;
                            break;
                        }
                    }

                    if (fin - inicio + 1 == frase.size()) {
                        // Encontramos un fragmento que contiene todas las palabras en orden
                        break;
                    }
                }
            }

            // Construir el fragmento
            std::ostringstream fragmento;
            for (int i = inicio; i <= fin; ++i) {
                fragmento << palabras_texto[i] << " ";
            }

            return fragmento.str();
        }*/


        //----------------------------------------------------------------------------------------------------------------------------------------------

        static bool compararPorValor(const std::pair<int, int>& a, const std::pair<int, int>& b) {
            return a.second > b.second; // Ordena de mayor a menor según el valor
        }


        //Funcion para sacar la interseccion de las paginas
        std::unordered_map<int, vector<int>> construirRankingPaginas(const std::unordered_map<int, std::vector<paginaStruct>>& hashPaginas) {

            unordered_map<int, vector<int>> rankingPaginasBueno;

            for(const auto &elementLibro : hashPaginas){

                std::unordered_map<int, int> rankingPaginas;

                // Llenar rankingPaginas
                for (const auto &elementStruct : elementLibro.second) {
                    const std::unordered_map<int, int> &hashPaginas = elementStruct.paginasApariciones;
                    for (const auto &numPagina : hashPaginas) {
                        rankingPaginas[numPagina.first]++;
                    }
                }

                // Convertir a un vector de pares (clave, valor)
                std::vector<std::pair<int, int>> vectorPares(rankingPaginas.begin(), rankingPaginas.end());

                // Ordenar el vector de pares según los valores
                std::sort(vectorPares.begin(), vectorPares.end(), &ProcesoIndexBusqueda::compararPorValor);

                // Crear un unordered_map ordenado según los valores
                std::unordered_map<int, int> rankingOrdenado;
                
                // Llenar el unordered_map, se va a llenar de forma ordenada
                vector<int> paginas;
                int contadorParrafos = 0;
                for (const auto &par : vectorPares) {
                    paginas.push_back(par.first);
                    contadorParrafos++;
                    if (contadorParrafos == 3){break;}
                    
                }

                rankingPaginasBueno[elementLibro.first] = paginas;

            }
            return rankingPaginasBueno;

            

            //Aqui ya se llama

        }

        


        //----------------------------------------------------------------------------------------------------------------------------------------------

        std::vector<std::pair<int, int>> construirRanking(std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<int, int>>> palabras) {
            std::unordered_map<int, int> ranking;


            // Itera sobre el resto de las palabras
            for (const auto& palabra : palabras) {
                const auto& libros = palabra.second;

                for (const auto& entry : libros) {
                    int libroId = entry.first;
                    // Actualiza el ranking sumando las apariciones
                    ranking[libroId]++;
                }
            }

            // Convertir el ranking a un vector de pares (clave, valor)
            std::vector<std::pair<int, int>> vectorPares(ranking.begin(), ranking.end());

            // Ordenar el vector de pares según los valores (apariciones)
            std::sort(vectorPares.begin(), vectorPares.end(), compararPorValor);

            


            return vectorPares;
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------



        //----------------------------------------------------------------------------------------------------------------------------------------------

        //Sin probar
        //Esta funcion srive para retornar un vector de int de aquellos valores que se encuentran repetidos en todos los otros vectores
        std::vector<int> encontrarValoresComunes(const std::vector<std::vector<int>>& vectores) {
            // Crea un conjunto para almacenar los valores comunes
            std::unordered_set<int> valoresComunes(vectores.at(0).begin(), vectores.at(0).end());

            // Itera sobre los vectores restantes y encuentra los valores comunes
            for (size_t i = 1; i < vectores.size(); ++i) {
                std::unordered_set<int> valoresEnEsteVector(vectores.at(i).begin(), vectores.at(i).end());

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
                int value = pvector.at(i);
                counts[value]++;
            }
            /*
            for (const auto& entry : counts) {
                std::cout << entry.first << " aparece " << entry.second << " veces" << std::endl;
            }
            */
            return counts;
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------

        //Prueba de indexacion
        std::unordered_map<int, int> crearHash2(vector<int> pvector){
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

        std::unordered_map <int, vector<fragmentoStruct>> getImpresionFinal(){
            return impresionFinal;
        }
};


#endif // PROCESOINDEXBUSQUEDA_