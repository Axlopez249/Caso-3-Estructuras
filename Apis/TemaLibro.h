#ifndef _TEMALIBRO_  
#define _TEMALIBRO_ 1
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class TemaLibro {
    private:
        vector<string> temas;
        string libro;

    public:
        PalabraLibro(vector<string> ptemas, string plibro){
            temas = ptemas;
            libro = plibro;
        }

        vector<string> getPalabra(){
            return temas;
        }

        string getLibro(){
            return libro;
        }
    
};

#endif