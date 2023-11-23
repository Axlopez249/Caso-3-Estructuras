#ifndef _CHAT_
#define _CHAT_ 1
#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <vector>
#include <sstream>

using json = nlohmann::json;
using namespace std;

class Chatgpt{
    private:
        string promptv;
        vector<string> palabras;
    public:

        Chatgpt(string pprompt){
            promptv = pprompt;
            preguntar();
        }
        // Función de callback para manejar la respuesta de la solicitud HTTP
        static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
            size_t total_size = size * nmemb;
            response->append((char*)contents, total_size);
            return total_size;
        }

        void parsearRespuesta(string& respuesta) {
            json jsonResponse = json::parse(respuesta);
            // Verifica si "choices" está presente y no está vacío
            if (jsonResponse.count("choices") > 0 && jsonResponse["choices"].is_array() && jsonResponse["choices"].size() > 0) {
                // Obtiene el contenido de la respuesta del asistente
                string respuestaAsistente = jsonResponse["choices"][0]["message"]["content"];
                
                // Reemplaza las comas por espacios en la respuesta
                std::replace(respuestaAsistente.begin(), respuestaAsistente.end(), ',', ' ');

                // Dividir la respuesta en palabras y almacenarlas en el vector
                istringstream iss(respuestaAsistente);
                std::cout << promptv << std::endl;
                palabras.assign(istream_iterator<string>(iss), istream_iterator<string>());
            } else {
                // Si no hay contenido en la respuesta del asistente, el vector de palabras se deja vacío
                std::cout << "2" << std::endl;
                palabras.clear();
            }
        }


        void preguntar() {
            CURL* curl;
            CURLcode res;

            // Configura la URL del endpoint de OpenAI
            std::string url = "https://api.openai.com/v1/chat/completions";

            // Define el prompt para el chat
            std::string prompt = "Usuario:"+ promptv +"\nAsistente: Mi nombre es GPT-3.5. ¿En qué puedo ayudarte hoy?";

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
                    //cout<<response_data<<endl;
                    parsearRespuesta(response_data);
                }

                // Limpia y cierra libcurl
                curl_easy_cleanup(curl);
                curl_global_cleanup();
            }

            
        }

        vector<string> getPalabras(){
            return palabras;
        }
};


#endif