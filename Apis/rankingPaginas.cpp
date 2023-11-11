    std::vector<paginaStruct> vectorStruct; // Supongamos que ya tienes este vector lleno
    std::unordered_map<int, int> rankingPaginas;

    // Llenar rankingPaginas
    for (const auto &elementStruct : vectorStruct) {
        const std::unordered_map<int, int> &hashPaginas = elementStruct.hashPaginas;
        for (const auto &numPagina : hashPaginas) {
            rankingPaginas[numPagina.first]++;
        }
    }

    // Convertir a un vector de pares (clave, valor)
    std::vector<std::pair<int, int>> vectorPares(rankingPaginas.begin(), rankingPaginas.end());

    // Ordenar el vector de pares según los valores
    std::sort(vectorPares.begin(), vectorPares.end(), compararPorValor);

    // Crear un multimap ordenado según los valores
    std::unordered_map<int, int> rankingOrdenado;

    // Llenar el multimap
    for (const auto &par : vectorPares) {
        rankingOrdenado[par.first] = par.second;
    }

    // Imprimir el ranking ordenado por valores manteniendo las claves originales
    for (const auto &par : rankingOrdenado) {
        std::cout << "Página " << par.first << ": " << par.second << " apariciones\n";
    }