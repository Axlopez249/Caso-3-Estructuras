#ifndef CONFIGJSON
#define CONFIGJSON 1

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class ConfigJson
{
private:
    vector<string> palabras;

public:
    ConfigJson(json jsonData)
    {   
        // Parsea el string JSON
        
        // Acceder a los datos del objeto JSON
        //Para la lista de entidades que viene de la api
        auto entityList = jsonData["entity_list"];
        if (!entityList.empty() && entityList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como entidades osea nombres propios y más entonces van en un vector para la clave names

            for (const auto& entity : entityList) {
                string form = entity["form"];
                // Acceder a más datos según sea necesario
                palabras.push_back(form);
            }

        }

        //Para la lista de conceptos que viene de la api
        auto conceptList = jsonData["concept_list"];
        if (!conceptList.empty() && conceptList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como conceptos y más entonces van en un vector para la clave conceptos

            for (const auto& concept : conceptList) {
                string form = concept["form"];
                // Acceder a más datos según sea necesario
                palabras.push_back(form);
            }

        }

        //Para la lista de sujetos que viene de la api
        auto relationList = jsonData["relation_list"];
        if (!relationList.empty() && relationList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como conceptos y más entonces van en un vector para la clave conceptos

            for (const auto& relation : relationList) {
                auto complementList = relation["complement_list"];
                if (!complementList.empty() && complementList.is_array()) {
                    for (const auto& complement : complementLis)
                    {
                        string complementForm = complement["form"];
                        palabras.push_back(complementForm);
                    }
                }
                
                
                string subjectForm = relation["subject"]["form"];
                palabras.push_back(subjectForm);
            }

        }

    } 

    vector<string> getPalabras(){
        return palabras;
    }
    
};



#endif