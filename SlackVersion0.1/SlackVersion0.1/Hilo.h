#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Hilo
{
private:
    int id, idMensajePadre;

public:
    Hilo(int i = 0, int mensaje = 0) {
        id = i;
        idMensajePadre = mensaje;
    }

    // Getters
    int getId() const { return id; }
    int getIdMensajePadre() const { return idMensajePadre; }

    bool operator==(const Hilo& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Hilo& h) {
        os << "Hilo#" << h.id << " (Mensaje Base ID: " << h.idMensajePadre << ")";
        return os;
    }
};