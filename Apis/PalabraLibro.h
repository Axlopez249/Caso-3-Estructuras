#ifndef _LIBRO_  
#define _LIBRO_ 1
#include <string>
#include <iostream>

using namespace std;

class PalabraLibro {
    private:
        string palabra;
        string libro;

    public:
        PalabraLibro(string ppalabra, string plibro){
            palabra = ppalabra;
            libro = plibro;
        }

        string getPalabra(){
            return palabra;
        }

        string getLibro(){
            return libro;
        }
    
};

#endif