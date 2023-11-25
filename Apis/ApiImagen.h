#include <iostream>
#include <curl/curl.h>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class ApiImagen {
public:
    ApiImagen(const std::string& query){
        CURL* curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            std::string URL = "https://pixabay.com/api/?key=" + API_KEY + "&q=" + curl_easy_escape(curl, query.c_str(), query.length());

            curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
                response_data = "Error en la solicitud";
            }

            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
    }

    std::string getUrl(){
        return extraerPrimerURL(response_data);
    }

private:
    const std::string API_KEY = "40858339-9bb34558149c7b3dc83225f58";
    std::string response_data;

    static size_t write_callback(void* contents, size_t size, size_t nmemb, std::string* user_data) {
        user_data->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string extraerPrimerURL(const std::string& jsonString){
        try {
            json j = json::parse(jsonString);

            if (j["hits"].size() > 0) {
                std::string url = j["hits"][0]["pageURL"];
                return url;
            } else {
                return "No se encontraron URLs";
            }
        } catch (const std::exception& e) {

            return "Error al procesar el JSON: " + std::string(e.what());
        }
    }
};


