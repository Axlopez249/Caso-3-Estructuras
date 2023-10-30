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
    unordered_map<string, vector<string>> hashTable;

public:
    ConfigJson(json jsonData)
    {   
        // Parsea el string JSON
        
        // Acceder a los datos del objeto JSON
        //Para la lista de entidades que viene de la api
        auto entityList = jsonData["entity_list"];
        if (!entityList.empty() && entityList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como entidades osea nombres propios y más entonces van en un vector para la clave names
            vector<string> names;
            for (const auto& entity : entityList) {
                string form = entity["form"];
                // Acceder a más datos según sea necesario
                names.push_back(form);
            }
            hashTable["names"] = names;
        } else {
            std::cerr << "La lista de entidades está vacía o no es un array." << std::endl;
        }

        //Para la lista de conceptos que viene de la api
        auto conceptList = jsonData["concept_list"];
        if (!conceptList.empty() && conceptList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como conceptos y más entonces van en un vector para la clave conceptos
            vector<string> concepts;
            for (const auto& concept : conceptList) {
                string form = concept["form"];
                // Acceder a más datos según sea necesario
                names.push_back(form);
            }
            hashTable["concepts"] = concepts;
        } else {
            std::cerr << "La lista de concepts está vacía o no es un array." << std::endl;
        }

        //Para la lista de sujetos que viene de la api
        auto relationList = jsonData["relation_list"];
        if (!relationList.empty() && relationList.is_array()) {
            //Aqui se recorren todas las palabras que se van a extraer como conceptos y más entonces van en un vector para la clave conceptos
            vector<string> subjects;
            for (const auto& relation : relationList) {
                string subjectForm = relation["subject"]["form"];
                subjects.push_back(subjectForm);
            }
            hashTable["subjects"] = subjects;
        } else {
            std::cerr << "La lista de concepts está vacía o no es un array." << std::endl;
        }

    } 

    unordered_map<string, vector<string>> getHash(){
        return hashTable;
    }
    
};



#endif