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
#include "ExtractorPalabras.cpp"
#include "BPlusTree.h"
#include <map>
#include "paginaStruct.h"
#include "fragmentoStruct.h"
#include "fragmetoStructParrafo.h"
#include "LibroStruct.h"
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;
using namespace std;

class ProcesoIndexBusqueda{
    private:
        bool index = false;
        vector<std::unordered_map<std::string, std::vector<int>>> vectorHash; //vector de hash que tienen una palabra y las paginas y sus cantidad
        vector<string> libros; //Nombre de los libros
        vector<string> frase;
        vector<std::unordered_map<int, std::string>> vectorHashPaginas; //Vector de las paginas para ser metidas en el el b+
        std::vector<std::pair<int, int>> rankingLibros; //Ranking de libros
        std::unordered_map <int, vector<fragmentoStruct>> impresion;
        std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>> impresionFinal;

        AVLTree *arbol;

        vector<BPlusTree*> vectorArbolesB;
        
        //Instancia de clases
        diccHashTables *extractorPalabras = new diccHashTables();//Para extraer las palabras
//        AVLTree arbol; //arbol avl
    public:

        void ProcesoIndex() {    //pfrase es el vector de las palabras claves de la frase de busqueda
            
            vectorHash = extractorPalabras->getVectorHash();//Vector de hash
            libros = extractorPalabras->getLibros();

            

            arbol = new AVLTree();

            int indicePrimero = 1;


            //Primero recorro el vector que tenemos para poder ir ingresando los datos al arbol
            for (const auto& hash : vectorHash) {
                std::cout << indicePrimero << std::endl;
                indicePrimero++;
                for (const auto& elementHash : hash) {


                    //Datos para ingresarlos al nodo
                    string palabra = elementHash.first;
                    vector<int> vectorPaginas = elementHash.second;
                    unordered_map<int, int> hashInt = crearHash(vectorPaginas);
                    //Sacar indice del elemento
                    int indice = std::distance(vectorHash.begin(), std::find(vectorHash.begin(), vectorHash.end(), hash));
                    //Datos para ingresarlos al nodo
                    //Se agrega al arbol
                    arbol->insert(palabra, hashInt, 1, indice);
                    
                }
            }

            //Tengo este vector de hashtable de paginas
            vectorHashPaginas = extractorPalabras->getVectorHashPaginas();

//            int indiceAfuera = 0;
            //std::cout << "HAHA2" << std::endl;
            for (const auto& hash : vectorHashPaginas) {

                //Creo un arbol por cada libro
                BPlusTree *arbolB;

                arbolB = new BPlusTree();
                //For para poder recorrer cada hash
                //Dentro van a haber la clave (la palabra y su vector)

//                int indiceAdentro = 0;

                for (const auto& elementHash : hash) {

                    int numPagina = elementHash.first;
                    string contenidoPagina = elementHash.second;
                    //Datos para ingresarlos al nodo
                    //Se agrega al arbol
                    arbolB->insert(numPagina, contenidoPagina);
                    
                }
                 //Despues de indexar todas sus palabras me encargo de meterlo al vector
                vectorArbolesB.push_back(arbolB);
            }

            std::cout << "Indexacion lista" << endl;
            index = true;
            
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
        std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>> searchPrincipal(vector<string> pfrase){
            //Tengo cada palabra

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
            

            std::unordered_map<int, vector<int>> hashPaginasImportantes = construirRankingPaginas(hashPaginas); //el hash que guarda lo de esa funcion, guarda 3 paginas por cada libro

            //De aquí para abajo estará mi lógica, este comentario lo anoto por si es necesario borrar mi lógica debido a que es incorrecta

            frase = pfrase;

            impresion = busquedaBPlus(hashPaginasImportantes);
            //Ahora solo falta de alguna manera llamar a la funcion de busquedaBPlus para poder extraer el contenido de las pagina y sacar los parrafos que son
            impresionFinal = prepararImpresionFinal(impresion);


            return impresionFinal;
        }

        //----------------------------------------------------------------------------------------------------------------------
        std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>> prepararImpresionFinal(std::unordered_map <int, vector<fragmentoStruct>> impresion){

            std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>> impresionFinal;

            for (const auto &libro : impresion)
            {
                string nameLibroAutor = libros.at(libro.first);              
                std::vector<fragmentoStructParrafo> fragmentos;
                for (const auto &pagina : libro.second)
                {
                    string contenidoPagina = pagina.contenido;
                    int numeroPagina = pagina.numPagina;
                    string fragmentoParrafo = getParrafo(contenidoPagina);
                    
                    fragmentoStructParrafo parrafo;
                    parrafo.contenido = fragmentoParrafo;
                    parrafo.numPagina = numeroPagina;
                    fragmentos.push_back(parrafo);
                }

                std::pair<string, std::vector<fragmentoStructParrafo>> nuevoPar(nameLibroAutor, fragmentos);
                impresionFinal.emplace_back(nuevoPar);

            }
            
            return impresionFinal;
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------

        std::string getParrafo(string pagina){

            std::vector<std::string> palabrasBusqueda = frase;
            std::string fragmento = "";
            std::vector<std::string> almacenamiento;
            std::string palabra;

            // Lee y almacena palabra por palabra
            std::istringstream ss(pagina);
            while (ss >> palabra) {
                almacenamiento.push_back(palabra);
            }

            //Recorro ahora las palabras del almacenamiento
            for (const auto &palabra_a_buscar : palabrasBusqueda) {
                auto iterador = std::find(almacenamiento.begin(), almacenamiento.end(), palabra_a_buscar);
                if (iterador != almacenamiento.end()) {
                    //std::cout << "La palabra '" << palabra_a_buscar << "' está en el vector." << std::endl;

                    size_t indice_encontrado = std::distance(almacenamiento.begin(), iterador);

                    size_t indice_inicio = (indice_encontrado > 20) ? indice_encontrado - 20 : 0;
                    size_t indice_final = std::min(indice_encontrado + 20, almacenamiento.size() - 1);

                    for (size_t i = indice_inicio; i <= indice_final; ++i) {
                        fragmento += almacenamiento[i];
                        fragmento += " ";
                    }
                    break;
                }
            }

            return fragmento;

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

            return pimpresionFinal;

        }

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

            std::vector<std::pair<int, int>> vectorCorrecto;
            int contador = 0;
            for (const auto &par : vectorPares)
            {
                vectorCorrecto.push_back(par);
                contador++;
                if (contador==10)
                {
                    break;
                }
                
            }
            return vectorCorrecto;
        }

        //----------------------------------------------------------------------------------------------------------------------------------------------



        //----------------------------------------------------------------------------------------------------------------------------------------------

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


        //funcion para indexar
        //esta funcion es para extraer del vector de paginas y colocarlo en un hashtable
        std::unordered_map<int, int> crearHash(vector<int> pvector){
            std::unordered_map<int, int> counts;

            for (int i = 0; i < pvector.size(); ++i) {
                int value = pvector.at(i);
                counts[value]++;
            }
            return counts;
        }


        //----------------------------------------------------------------------------------------------------------------------------------------------}

        bool getReadyIndex(){
            return index;
        }
};


#endif // PROCESOINDEXBUSQUEDA_