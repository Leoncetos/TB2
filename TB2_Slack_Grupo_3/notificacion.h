#pragma once
#include <iostream>

using namespace std;

class Notificacion {
public:
    int id;
    int idUsuario;
    int idMensaje;
    string tipo;
    bool leida;

    Notificacion(int id = 0, int idU = 0, int idM = 0, string t = "mensaje_nuevo", bool l = false) {
        this->id = id;
        this->idUsuario = idU;
        this->idMensaje = idM;
        this->tipo = t;
        this->leida = l;
    }
};