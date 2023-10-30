#include <iostream>
#include <string>
#include <curl/curl.h>

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
    size_t total_size = size * nmemb;
    output->append((char *)contents, total_size);
    return total_size;
}

int main()
{
    CURL *curl;
    CURLcode res;
    std::string api_key = "a1f14c7776msh0f003f7c799c89dp1db39cjsn6afe4692626a"; // Reemplaza con tu clave de API de WordsAPI
    std::string response_data;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl)
    {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("X-RapidAPI-Key: " + api_key).c_str());
        headers = curl_slist_append(headers, "X-RapidAPI-Host: wordsapiv1.p.rapidapi.com");

        curl_easy_setopt(curl, CURLOPT_URL, "https://wordsapiv1.p.rapidapi.com/words/example/synonyms");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            std::cout << "Respuesta de la API: " << response_data << std::endl;
        }
        else
        {
            std::cerr << "Error en la solicitud: " << curl_easy_strerror(res) << std::endl;
        }
    }

    curl_global_cleanup();
    return 0;
}
