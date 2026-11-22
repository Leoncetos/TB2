#pragma once
#include <iostream>

class Hilo {
public:
    int id;
    int idMensajePadre;

    Hilo(int id = 0, int idPadre = 0) {
        this->id = id;
        this->idMensajePadre = idPadre;
    }
};