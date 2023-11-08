
#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"
#include <curl/curl.h>
#include "ArbolTitulo.cpp"
#include "Chatgpt.cpp"
#include <unordered_map>

using json = nlohmann::json;
using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

// docker run -it --rm -v C:\Users\PRINCIPAL\Desktop\caso3\Caso-3-Estructuras\Apis:/home -p 8080:8080 gcc bash

int main() {
    httplib::Server svr;

    svr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
        std::string input_text = req.body;  // La frase de búsqueda proviene del cuerpo del mensaje HTTP

        std::cout << input_text << std::endl;

        string prompt = "Quiero que tu respuesta sea solamente las palabras claves de esta frase: " + input_text + ". Sin texto extra por tu parte";
        Chatgpt chat(prompt);

        vector<string> clavesSolicitud = chat.getPalabras(); 
        
        ArbolTitulo* arbol = new ArbolTitulo(clavesSolicitud);

        std::map<std::string, int, std::greater<>> ranking = arbol->ranking;

        // Convertir el mapa a un objeto JSON
        json jsonResponse = ranking;

        // Convertir el objeto JSON a una cadena
        std::string responseBody = jsonResponse.dump();

        // Responder con el objeto JSON en el cuerpo de la respuesta HTTP
        res.set_content(responseBody, "application/json");
    });

    std::cout << "Servidor en ejecución en el puerto 8080" << std::endl;
    svr.listen("localhost", 8080);

    return 0;
}