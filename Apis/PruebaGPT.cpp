#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

// Función de callback para manejar la respuesta de la solicitud HTTP
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t total_size = size * nmemb;
    response->append((char*)contents, total_size);
    return total_size;
}

int main() {
    CURL* curl;
    CURLcode res;

    // Configura la URL del endpoint de OpenAI
    std::string url = "https://api.openai.com/v1/chat/completions";

    // Define el prompt para el chat
    std::string prompt = "Usuario: Hola, ¿cuál es tu nombre?\nAsistente: Mi nombre es GPT-3.5. ¿En qué puedo ayudarte hoy?";

    // Configura la clave de API
    std::string api_key = "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";

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

        std::string json_data = request_body.dump();

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

        // Configura la función de callback para manejar la respuesta
        std::string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        // Realiza la solicitud HTTP
        res = curl_easy_perform(curl);

        // Verifica si la solicitud fue exitosa
        if (res != CURLE_OK) {
            fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
        } else {
            // Muestra la respuesta
            std::cout << "Respuesta del asistente: " << response_data << std::endl;
        }

        // Limpia y cierra libcurl
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    return 0;
}