#pragma once
#include <iostream>
#include <string>
using namespace std;

class Notificacion
{
private:
    int id, idUsuario, idMensaje;
    string tipo;
    bool leida;

public:
    Notificacion(int i = 0, int usuario = 0, int mensaje = 0, string t = "") {
        id = i;
        idUsuario = usuario;
        idMensaje = mensaje;
        tipo = t;
        leida = false;
    }

    // Getters
    int getId() const { return id; }
    int getIdUsuario() const { return idUsuario; }
    int getIdMensaje() const { return idMensaje; }
    string getTipo() const { return tipo; }
    bool isLeida() const { return leida; }

    // Setters
    void marcarLeida() { leida = true; }

    bool operator==(const Notificacion& otra) const { return id == otra.id; }

    friend ostream& operator<<(ostream& os, const Notificacion& notificacion) {
        os << "Notificacion#" << notificacion.id << " [Usuario:" << notificacion.idUsuario
           << "] " << notificacion.tipo
           << (notificacion.leida ? " [LEIDA]" : " [PENDIENTE]");
        return os;
    }
};