#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Mensaje
{
private:
    int id, idAutor, idCanal, idPadre;
    string contenido, fecha;

public:
    Mensaje(int i = 0, int autor = 0, int canal = 0, string texto = "",
            string f = "", int padre = 0) {
        id = i;
        idAutor = autor;
        idCanal = canal;
        idPadre = padre;
        contenido = texto;
        fecha = f;
    }

    ~Mensaje() = default;

    // Getters
    int getId() const { return id; }
    int getIdAutor() const { return idAutor; }
    int getIdCanal() const { return idCanal; }
    int getIdPadre() const { return idPadre; }
    string getContenido() const { return contenido; }
    string getFecha() const { return fecha; }

    // Setters
    void setContenido(const string& texto) { contenido = texto; }

    bool operator==(const Mensaje& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Mensaje& m) {
        os << "Msg#" << m.id << " [Canal:" << m.idCanal << ", Autor:" << m.idAutor
           << "] " << m.fecha << ": " << m.contenido;
        return os;
    }
};