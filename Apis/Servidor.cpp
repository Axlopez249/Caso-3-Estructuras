#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

void handle_get(http_request request) {
    ucout << request.to_string() << std::endl;
    json::value response;
    response[U("message")] = json::value::string(U("Hello, World!"));
    request.reply(status_codes::OK, response);
}

void handle_post(http_request request) {
    ucout << request.to_string() << std::endl;

    request.extract_json()
        .then([&request](json::value body) {
            json::value response;
            response[U("message")] = json::value::string(U("Received POST request with data: ") + body.serialize());
            request.reply(status_codes::OK, response);
        })
        .wait();
}

int main() {
    http_listener listener(U("http://localhost:8080/api"));
    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);

    try {
        listener.open().wait();
        ucout << U("Listening for requests at: ") << listener.uri().to_string() << std::endl;
        std::cin.get();
        listener.close().wait();
    } catch (const std::exception& e) {
        ucout << U("Error: ") << e.what() << std::endl;
    }

    return 0;
}
