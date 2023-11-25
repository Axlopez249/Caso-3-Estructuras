#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "json.hpp"
#include "ProcesoIndexBusqueda.cpp"
#include <vector>
#include <algorithm>
#include <sstream>
#include "Chatgpt.cpp"
#include "ApiImagen.h"

using json = nlohmann::json;

const int PORT = 8086;

class Servidor {
public:
    static Servidor crearServidor() {
        return Servidor();
    }
    
    void ejecutar(ProcesoIndexBusqueda *proceso) {
        int serverSocket, clientSocket;
        struct sockaddr_in serverAddress, clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);

        // Crear un socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            perror("Error al crear el socket");
        }

        // Configurar la dirección del servidor
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(PORT);

        // Enlazar el socket a la dirección y puerto
        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
            perror("Error al enlazar el socket");
        }

        // Escuchar conexiones entrantes
        if (listen(serverSocket, 5) < 0) {
            perror("Error al escuchar");
        }

        std::cout << "Servidor HTTP escuchando en el puerto " << PORT << std::endl;

        while (true) {
            // Aceptar una conexión entrante
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

            if (clientSocket < 0) {
                perror("Error al aceptar la conexión");
                continue;
            }

            char buffer[1024];
            std::memset(buffer, 0, sizeof(buffer));

            // Leer la solicitud HTTP del cliente
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesRead < 0) {
                perror("Error al leer la solicitud HTTP");
                close(clientSocket);
                continue;
            }

            // Analizar la solicitud HTTP para encontrar el cuerpo
            std::string request(buffer);
            size_t bodyStart = request.find("\r\n\r\n");
            size_t requestStart = request.find(" ") + 1; // Encuentra el inicio de la URL
            size_t requestEnd = request.find(" ", requestStart); // Encuentra el final de la URL
            std::string requestURL = request.substr(requestStart, requestEnd - requestStart); // Extrae la URL

            // Verifica si la solicitud es para '/api/busqueda'
            if (requestURL == "/api/busqueda") {
                // Si la solicitud es para '/api/busqueda', maneja esta ruta
                // Extraer el cuerpo de la solicitud

                

                std::string requestBody = request.substr(bodyStart + 4);
                cout << "cuerpo del json de recibida: " + requestBody << endl;

                if(requestBody.compare("{\"frase\":\"\"}") != 0) {
                    std::vector<string> frase = obtenerFraseDeBusqueda(requestBody);
                    cout << "" << endl;
                    for (const auto &palabr : frase)
                    {
                        cout << "palabra de la frase: " + palabr << endl;
                    }
                    
                    
                    std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>> informacion = proceso->searchPrincipal(frase);
                    
                    //Llamo mi clase de chatgpt
                    Chatgpt extractorGenero;
                    Chatgpt extractorImagen;

                    //Extraigo el genero de la frase con la clase
                    
                    std::string promptGenero = "Dame un genero de estos: negativo, positivo, neutro, misterioso, novela, fantasía relacionado para esta frase: " + vfrase;
                    std::string promptImagen = "Toma un sustantivo o la palabra mas importante de esta frase: " + vfrase;
                    
                    extractorGenero.getRespuesta(promptGenero);
                    std::string genero = extractorGenero.getGenero();
                    cout << "" << endl;
                    cout << "El genero es " + genero << endl;

                    extractorImagen.getRespuesta(promptImagen);
                    std::string palabraBusquedaImagen = extractorImagen.getPalabraImportante(frase);

                    cout << "la palabra para la imagen es: " +  palabraBusquedaImagen << endl;

                    ApiImagen pixabay(palabraBusquedaImagen);
                    std::string imagen = pixabay.getUrl();
                    cout << "el url de la imagen es: " +  imagen << endl;
                    // Responder con el JSON como la respuesta de la solicitud
                    std::string responseBody = construirRespuestaJSON(informacion, genero, imagen);

                    cout << "La respuesta final es: "<< endl;

                    cout << responseBody << endl;

                    //close(clientSocket);
                    // Responder con la respuesta JSON
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n";
                    response += "Access-Control-Allow-Origin: *\r\n";
                    response += "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
                    response += "Access-Control-Allow-Headers: Content-Type\r\n";
                    response += "\r\n" + responseBody;

                    send(clientSocket, response.c_str(), response.size(), 0);
                    
                } else {
                    cout << "Entro a caso donde no hay nada" << endl;
                }

                
            } else {
                // Si la solicitud no es para '/api/busqueda', maneja otras rutas si es necesario

                // Respondemos con un mensaje indicando una solicitud no encontrada para esa ruta
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nLa ruta solicitada no fue encontrada.";
                send(clientSocket, response.c_str(), response.size(), 0);
            }

            // Cerrar la conexión con el cliente
            close(clientSocket);
        }

        // Cerrar el socket del servidor
        close(serverSocket);
    }

private:
    std::string vfrase;
    std::string palabraBusquedaImagen;
    Servidor() {}

    std::vector<string> obtenerFraseDeBusqueda(const std::string& requestBody) {
        std::vector<string> palabrasFrase;
        
        try {
            json bodyJson = json::parse(requestBody);

            // Verificar si el campo "frase" existe en el JSON
            if (bodyJson.contains("frase")) {
                string frase = bodyJson["frase"];
                vfrase = frase;
                std::string palabra;

                std::istringstream ss(frase);
                while (ss >> palabra) {
                    if (palabra.length() > 3) {
                        palabrasFrase.push_back(palabra);
                    }
                }
            }
        } catch (const json::parse_error& e) {
            // Manejar el error de parseo del JSON
            // Puedes imprimir el mensaje de error e informar sobre el problema
            //std::cerr << "Error al parsear el JSON: " << e.what() << std::endl;
        }
        
        return palabrasFrase;
    }


    std::string construirRespuestaJSON(const std::vector<std::pair<string, std::vector<fragmentoStructParrafo>>>& informacion, string genero, string imagen) {
        json jsonResponse;

        // Agregar tipoGenero y UrlImagen al JSON
        jsonResponse["tipoGenero"] = genero;
        jsonResponse["UrlImagen"] = imagen;

        for (const auto& libroInfo : informacion) {
            string libroAutor = libroInfo.first;
            const std::vector<fragmentoStructParrafo>& fragmentos = libroInfo.second;

            json fragmentosJson;

            for (const auto& fragmento : fragmentos) {
                fragmentosJson[std::to_string(fragmento.numPagina)] = fragmento.contenido;
            }

            jsonResponse[libroAutor] = fragmentosJson;
        }

        return jsonResponse.dump();
    }

};

int main() {
    
    std::cout << "El programa empieza" << endl;

    ProcesoIndexBusqueda *proceso = new ProcesoIndexBusqueda();
    proceso->ProcesoIndex();
    
    
    if (proceso->getReadyIndex()==true)
    {   
        Servidor servidor = Servidor::crearServidor();
        servidor.ejecutar(proceso);   
    }
      
    

    std::cout << "Fin del programa" << endl;
    
    

    return 0;
}
