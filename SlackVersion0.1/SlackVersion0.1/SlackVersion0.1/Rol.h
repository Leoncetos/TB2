#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Rol
{
private:
    int id;
    string nombre, descripcion;

public:
    Rol(int i = 0, string n = "", string d = "") {
        id = i;
        nombre = n;
        descripcion = d;
    }

    // Getters
    int getId() const { return id; }

    bool operator==(const Rol& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Rol& r) {
        os << "Rol#" << r.id << " " << r.nombre << " (" << r.descripcion << ")";
        return os;
    }
};