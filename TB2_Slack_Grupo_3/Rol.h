#pragma once
#include <iostream>
#include <string>

using namespace std;

class Rol {
public:
    int id;
    string nombre;
    string descripcion;

    Rol(int id = 1, string nom = "Miembro", string desc = "Usuario estandar del espacio") {
        this->id = id;
        this->nombre = nom;
        this->descripcion = desc;
    }
};