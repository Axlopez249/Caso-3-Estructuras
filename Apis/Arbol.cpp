#include <iostream>
#include "ChatGPT/include/ChatGPT.h"
#include "ChatGPT/include/Error.h"
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int main() {
    std::string folderPath = "Biblioteca"; // Reemplaza con la ruta de tu carpeta
    std::vector<std::string> fileNames;

    try {
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                fileNames.push_back(entry.path().filename());
            }
        }

        // Imprimir los nombres de los archivos almacenados en el vector
        for (const auto& fileName : fileNames) {
            std::cout << fileName << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}

#include <iostream>


int main(int args,char** argv){
    //API token as argument
    OpenAI::ChatGPT chatGpt{argv[1]};
    try {
        //type of user and the message to ask
        auto response = chatGpt.askChatGPT("user","Give me 5 words");
        //Iterate all answers
        for(const auto& choice:response.choices){
            std::cout<<choice.message.content;
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
    try {
        auto response = chatGpt.askWhisper("sound.mp3");
        std::cout<<response;
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}
