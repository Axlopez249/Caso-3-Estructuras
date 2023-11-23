#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

int main(){
    std::string pagina = "My daughter Christine, who wrote me these letters, died at a hospital in Stuttgart on the morning of August 8th, 1914, of acute double pneumonia. I have kept the letters private for nearly three years, because, apart from the love in them that made them sacred things in days when we each still hoarded what we had of good, they seemed to me, who did not know the Germans and thought of them, as most people in England for a long while thought, without any bitterness and with a great inclination to explain away and excuse, too extreme and sweeping in their judgments. Now, as the years have passed, and each has been more full of actions on Germany's part difficult to explain except in one way and impossible to excuse, I feel that these letters, giving a picture of the state of mind of the German public immediately before the War, and written by some one who went there enthusiastically ready to like everything and everybody, may have a certain value in helping to put together a small corner of the great picture of Germany which it will be necessary to keep clear and naked before us in the future if the world is to be saved.";

    std::vector<std::string> palabrasBusqueda = {"Germans", "people", "England"};
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
            std::cout << "La palabra '" << palabra_a_buscar << "' está en el vector." << std::endl;

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

    std::cout << fragmento << std::endl;

    return 0;
}
