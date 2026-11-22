#pragma once
#include <iostream>

using namespace std;

class RegistroActividad {
public:
    int id;
    int idUsuario;
    string fecha;
    string accion;
    string detalle;

    RegistroActividad(int id = 0, int idU = 0, string f = "", string acc = "", string det = "") {
        this->id = id;
        this->idUsuario = idU;
        this->fecha = f;
        this->accion = acc;
        this->detalle = det;
    }
};