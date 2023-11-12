#ifndef MAIN_
#define MAIN_

#include <iostream>
#include "ProcesoIndexBusqueda.cpp" // Asegúrate de incluir correctamente el archivo de encabezado

int main() {
    // Crear una instancia de la clase diccHashTables
    diccHashTables diccionario;

    // Obtener información de las hashtables y los libros
    auto vectorHash = diccionario.getVectorHash();
    auto vectorHashPaginas = diccionario.getVectorHashPaginas();
    auto libros = diccionario.getLibros();

    // Imprimir información de las hashtables y los libros
    std::cout << "Información de hashtables:" << std::endl;
    for (size_t i = 0; i < vectorHash.size(); ++i) {
        std::cout << "Libro #" << i << ":" << std::endl;
        for (const auto& element : vectorHash[i]) {
            std::cout << "Palabra: " << element.first << ", Páginas: ";
            for (int pagina : element.second) {
                std::cout << pagina << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Información de páginas:" << std::endl;
    for (size_t i = 0; i < vectorHashPaginas.size(); ++i) {
        std::cout << "Libro #" << i << ":" << std::endl;
        for (const auto& element : vectorHashPaginas[i]) {
            std::cout << "Página: " << element.first << ", Contenido: " << element.second << std::endl;
        }
        std::cout << std::endl;
    }

    std::cout << "Lista de libros:" << std::endl;
    for (const auto& libro : libros) {
        std::cout << libro << std::endl;
    }

    // Crear una instancia de la clase ProcesoIndexBusqueda
    ProcesoIndexBusqueda procesoIndex;

    // Aquí puedes realizar pruebas adicionales o llamadas a funciones de ProcesoIndexBusqueda

    return 0;
}

#endif