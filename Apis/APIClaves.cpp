#ifndef _APICLAVES_  
#define _APICLAVES_ 1
#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include "ExtraccionJson.cpp"

using json = nlohmann::json;

class PalabrasClaves
{
    private:
        string libro;
        vector<string> palabras;

    public:
        PalabrasClaves(string plibro){
            libro = plibro;
            conexion();
        }

        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
            size_t total_size = size * nmemb;
            output->append(static_cast<char*>(contents), total_size);
            return total_size;
        }

        void conexion() {
            // Definir la frase de entrada y otros parámetros
            std::string input_text = libro;
            std::string api_key = "605b20455715f7fde1bb75c5474b1dfb";
            std::string language = "en";  // Código de idioma de 2 letras, como en, es, fr, etc.
            std::string topics = "a";  // "a" para todos los temas

            // Configurar la URL de la API
            std::string api_url = "https://api.meaningcloud.com/topics-2.0";

            // Crear una solicitud POST con cURL
            CURL* curl = curl_easy_init();
            if (curl) {
                // Construir los datos del formulario
                std::string post_fields = "key=" + api_key + "&txt=" + input_text + "&lang=" + language + "&tt=" + topics;

                curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

                // Almacenar la respuesta
                std::string response;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

                // Realizar la solicitud
                CURLcode res = curl_easy_perform(curl);

                // Verificar el resultado de la solicitud
                if (res == CURLE_OK) {
                    // Parsear la respuesta JSON
                    json jsonData = json::parse(response);
                    ConfigJson parseador(jsonData);
                    palabras = parseador.getPalabras();
                    
                   
                } else {
                    std::cerr << "Error en la solicitud cURL: " << curl_easy_strerror(res) << std::endl;
                }

                // Limpieza y liberación de recursos
                curl_easy_cleanup(curl);
            }

        }

        vector<string> getPalabras(){
            return palabras;
        }

};

#endif