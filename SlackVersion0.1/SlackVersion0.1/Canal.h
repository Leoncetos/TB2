#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Canal
{
protected:
    int id;
    string nombre;
    int cantidadMensajes;

public:
    Canal(int i = 0, string n = "", int cm = 0) {
        id = i;
        nombre = n;
        cantidadMensajes = cm;
    }

    virtual ~Canal() = default;

    // Getters
    int getId() const { return id; }
    string getNombre() const { return nombre; }
    int getCantidadMensajes() const { return cantidadMensajes; }
    virtual string getTipo() const = 0;
    virtual bool puedeEntrar(int idUsuario) const = 0;
    virtual bool agregarUsuario(int idUsuario) = 0;
    virtual string getAccesos() const = 0;

    // Setters
    void incrementarMensajes() { cantidadMensajes++; }

    bool operator==(const Canal& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Canal& c) {
        os << "Canal#" << c.id << " #" << c.nombre << " (" << c.getTipo()
           << ") - Msgs: " << c.cantidadMensajes;
        return os;
    }
};