#pragma once
#include <iostream>


using namespace std;


class Estado {
public:
    int id;
    int idUsuario;
    string texto;
    string emoji;

    Estado(int id = 1, int idU = 0, string txt = "activo", string em = ":online:") {
        this->id = id;
        this->idUsuario = idU;
        this->texto = txt;
        this->emoji = em;
    }
};