#ifndef _CHAT_
#define _CHAT_ 1

#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>
#include <vector>

using json = nlohmann::json;
using namespace std;

class Chatgpt {
private:
    string promptv;
    string respuesta;

public:

    // Función de callback para manejar la respuesta de la solicitud HTTP
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
        size_t total_size = size * nmemb;
        response->append((char*)contents, total_size);
        return total_size;
    }

    void getRespuesta(string promptv) {
        CURL* curl;
        CURLcode res;
        string respuestaAsistente;

        // Configura la URL del endpoint de OpenAI
        string url = "https://api.openai.com/v1/chat/completions";

        // Define el prompt para el chat
        string prompt = "Usuario:" + promptv + "\nAsistente: Mi nombre es GPT-3.5. ¿En qué puedo ayudarte hoy?";

        // Configura la clave de API
        string api_key = "";

        // Inicializa libcurl
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            // Configura la solicitud POST
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/json"); // Especifica el tipo de contenido JSON
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Configura el cuerpo de la solicitud JSON utilizando json.hpp
            json request_body = {
                {"messages", {
                    {{"role", "system"}, {"content", "You are a helpful assistant."}},
                    {{"role", "user"}, {"content", prompt}}
                }},
                {"model", "gpt-3.5-turbo"}
            };

            string json_data = request_body.dump();
            //cout << json_data << endl;

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

            // Configura la función de callback para manejar la respuesta
            string response_data;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

            // Realiza la solicitud HTTP
            res = curl_easy_perform(curl);

            // Verifica si la solicitud fue exitosa
            if (res != CURLE_OK) {
                fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
            } else {
                // Muestra la respuesta
                json jsonResponse = json::parse(response_data);
                if (jsonResponse.count("choices") > 0 && jsonResponse["choices"].is_array() && jsonResponse["choices"].size() > 0) {
                    respuestaAsistente = jsonResponse["choices"][0]["message"]["content"];
                    //cout << respuestaAsistente << endl;
                }
                
            }

            // Limpia y cierra libcurl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
        }

        respuesta = respuestaAsistente;
    }

    string getGenero(){
        string genero;
        //Lo primero que tengo que definir es un vector que tenga todas las respduestas posibles
        vector<string> generos = {"negativo", "positivo", "neutro", "misterioso", "novela", "fantasía"};

        std::istringstream ss(respuesta);
        std::string palabra;
        while (ss >> palabra) {
            std::string palabra_normalizada = normalizeWord(palabra);
            auto it = std::find(generos.begin(), generos.end(), palabra_normalizada);
            if (it != generos.end()) {
                genero = palabra_normalizada;
                break;
            }
        }

        return genero;
    }

    string getPalabraImportante(std::vector<string> ppalabras){
        string palabraImportante;
        //Lo primero que tengo que definir es un vector que tenga todas las respduestas posibles
        vector<string> palabras = ppalabras;

        std::istringstream ss(respuesta);
        std::string palabra;
        while (ss >> palabra) {
            std::string palabra_normalizada = normalizeWord(palabra);
            auto it = std::find(palabras.begin(), palabras.end(), palabra_normalizada);
            if (it != palabras.end()) {
                palabraImportante = palabra_normalizada;
                break;
            }
        }

        return palabraImportante;
    }

    std::string normalizeWord(const std::string& word) {
        std::string normalized = word;
        
        // Convertir a minúsculas
        std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        
        // Eliminar caracteres no deseados (por ejemplo, puntos, comas, etc.)
        normalized.erase(std::remove_if(normalized.begin(), normalized.end(), [](unsigned char c) {
            return std::ispunct(c);
        }), normalized.end());

        return normalized;
    }

    /*string getResumen(string frase){

    }*/
};
/*
int main(){
    Chatgpt g;
    g.getRespuesta("Toma un sustantivo o la palabra mas importante de esta frase: the night is beautiful. No me des explicaciones");
    std::vector<string> vectorPalabras = {"the", "night", "is", "beautiful"};
    std::cout << g.getPalabraImportante(vectorPalabras) << endl;
}*/

#endif
