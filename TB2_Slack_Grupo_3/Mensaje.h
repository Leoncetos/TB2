#include <iostream>

class Mensaje {
public:
    int id;
    int idAutor;
    int idCanal;
    int idPadre;
    string tipoContenido;
    string contenido;
    string fecha;

    Mensaje(int id = 0, int autor = 0, int canal = 0, int padre = 0, string tipo = "texto", string cont = "", string f = "") {
        this->id = id;
        this->idAutor = autor;
        this->idCanal = canal;
        this->idPadre = padre;
        this->tipoContenido = tipo;
        this->contenido = cont;
        this->fecha = f;
    }
};