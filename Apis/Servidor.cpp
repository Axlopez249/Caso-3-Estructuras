
#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"
#include <curl/curl.h>
#include "ExtraccionJson.cpp"
#include "CompararFrase.cpp"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

int main() {
    httplib::Server svr;

    svr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
        std::string input_text = req.body;  // La frase de búsqueda proviene del cuerpo del mensaje HTTP

        // Configurar la URL de la API y la clave de API de MeaningCloud
        std::string api_url = "https://api.meaningcloud.com/topics-2.0";
        std::string api_key = "605b20455715f7fde1bb75c5474b1dfb";  // Reemplaza con tu clave de API de MeaningCloud

        // Crear una solicitud POST con cURL
        CURL* curl = curl_easy_init();
        if (curl) {
            // Construir los datos del formulario
            std::string post_fields = "key=" + api_key + "&txt=" + input_text + "&lang=en&tt=a";

            curl_easy_setopt(curl, CURLOPT_URL, api_url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());

            // Almacenar la respuesta
            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            // Realizar la solicitud
            CURLcode res_code = curl_easy_perform(curl);

            // Verificar el resultado de la solicitud
            if (res_code == CURLE_OK) {
                json jsonData = json::parse(response);
                ConfigJson parseador(jsonData);
                palabras = parseador.getPalabras();

                //Se llama a CompararFrase para que me compare todas las cosas
                CompararFrase comparador = new CompararFrase(palabras);


                // Responder con la respuesta del API de MeaningCloud
                res.set_content(response, "application/json");
            } else {
                std::cerr << "Error en la solicitud cURL: " << curl_easy_strerror(res_code) << std::endl;
                res.set_content("Error en la solicitud a MeaningCloud.", "text/plain");
            }

            // Limpieza y liberación de recursos
            curl_easy_cleanup(curl);
        } else {
            std::cerr << "Error al inicializar cURL." << std::endl;
            res.set_content("Error en la solicitud a MeaningCloud.", "text/plain");
        }
    });

    std::cout << "Servidor en ejecución en el puerto 8080" << std::endl;
    svr.listen("localhost", 8080);

    return 0;
}