#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>  // Para std::remove_if
#include <cctype>     // Para std::tolower

int main() {
    std::ifstream archivo("prueba.txt"); // Reemplaza "libro.txt" con el nombre de tu archivo
    if (archivo.is_open()) {
        std::string linea;
        std::unordered_map<std::string, int> hashtable;

        while (std::getline(archivo, linea)) {
            std::istringstream iss(linea);
            std::string palabra;

            while (iss >> palabra) {
                // Limpiar la palabra de caracteres no alfabéticos y convertirla a minúsculas
                palabra.erase(std::remove_if(palabra.begin(), palabra.end(), [](char c) { return !isalpha(c); }), palabra.end());
                for (char& c : palabra) {
                    c = std::tolower(c);
                }

                // Insertar la palabra en el hashtable
                if (!palabra.empty()) {
                    hashtable[palabra]++;
                }
            }
        }

        archivo.close();

        // Ahora el hashtable contiene las palabras y su frecuencia de aparición en el libro
        // Puedes acceder a los valores utilizando hashtable["palabra"]
        
        // Imprimir las palabras y sus frecuencias
        for (const auto& pair : hashtable) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    } else {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
    }

    return 0;
}
