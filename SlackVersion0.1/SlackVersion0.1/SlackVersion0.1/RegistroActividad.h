#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class RegistroActividad
{
private:
    int id, idUsuario;
    string fecha, accion, detalle;

public:
    RegistroActividad(int i = 0, int usuario = 0, string momento = "",
                      string tipoAccion = "", string informacion = "") {
        id = i;
        idUsuario = usuario;
        fecha = momento;
        accion = tipoAccion;
        detalle = informacion;
    }

    // Getters
    int getId() const { return id; }
    int getIdUsuario() const { return idUsuario; }
    string getFecha() const { return fecha; }
    string getAccion() const { return accion; }
    string getDetalle() const { return detalle; }

    bool operator==(const RegistroActividad& otro) const { return id == otro.id; }

    friend ostream& operator<<(ostream& os, const RegistroActividad& registro) {
        os << "[" << registro.fecha << "] ";
        if (registro.idUsuario == 0) os << "Sistema";
        else os << "Usuario " << registro.idUsuario;
        os << " - " << registro.accion << ": " << registro.detalle;
        return os;
    }
};