#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"
#include <sstream>

using json = nlohmann::json;

class Synonyms {
private:
    std::string palabra;
    std::vector<std::string> sinonimos;

public:
    Synonyms(const std::string& ppalabra) : palabra(ppalabra) {}

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char*>(contents), total_size);
        return total_size;
    }

    void extraccion() {
        CURL* curl;
        CURLcode res;
        std::string api_key = "a1f14c7776msh0f003f7c799c89dp1db39cjsn6afe4692626a"; // Reemplaza con tu clave de API de WordsAPI
        std::string response_data;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, ("X-RapidAPI-Key: " + api_key).c_str());
            headers = curl_slist_append(headers, "X-RapidAPI-Host: wordsapiv1.p.rapidapi.com");
            std::stringstream url;
            url << "https://wordsapiv1.p.rapidapi.com/words/" << palabra << "/synonyms";

            curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

            res = curl_easy_perform(curl);

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                try {
                    json jsonData = json::parse(response_data);
                    if (jsonData.find("synonyms") != jsonData.end() && jsonData["synonyms"].is_array()) {
                        for (const auto& sinonimo : jsonData["synonyms"]) {
                            sinonimos.push_back(sinonimo.get<std::string>());
                        }
                    }
                } catch (const std::exception& ex) {
                    std::cerr << "Error al parsear JSON: " << ex.what() << std::endl;
                }
            } else {
                std::cerr << "Error en la solicitud: " << curl_easy_strerror(res) << std::endl;
            }
        }

        curl_global_cleanup();
    }

    vector<string> getSinonimos(){
        return sinonimos;
    }
};

int main() {
    std::string palabra = "example"; // Palabra de la que deseas obtener sinónimos
    Synonyms extractorSinonimos(palabra);
    extractorSinonimos.extraccion();
    extractorSinonimos.mostrarSinonimos();

    return 0;
}