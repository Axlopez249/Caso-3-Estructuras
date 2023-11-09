#include <iostream>
#include <string>
#include "httplib.h"

int main() {
    httplib::Server svr;

    svr.Post("/process", [](const httplib::Request& req, httplib::Response& res) {
        std::string input_text = req.body;  // La frase de búsqueda proviene del cuerpo del mensaje HTTP

        // Imprimir la frase de búsqueda en la consola
        std::cout << "Frase de búsqueda recibida: " << input_text << std::endl;

        // Responder con un mensaje de éxito
        res.set_content("Frase de búsqueda recibida correctamente.", "text/plain");
    });

    std::cout << "Servidor en ejecución en el puerto 8000" << std::endl;
    svr.listen("localhost", 8000);

    return 0;
}
