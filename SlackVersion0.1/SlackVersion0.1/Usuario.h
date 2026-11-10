#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Usuario
{
private:
    int id;
    string nombre, email, estado, rol;

public:
    Usuario(int i = 0, string n = "", string e = "", string s = "activo", string r = "Miembro") {
        id = i;
        nombre = n;
        email = e;
        estado = s;
        rol = r;
    }

    ~Usuario() = default;

    // Getters
    int getId() const { return id; }
    string getNombre() const { return nombre; }
    string getEmail() const { return email; }
    string getEstado() const { return estado; }
    string getRol() const { return rol; }

    // Setters
    void setEstado(const string& s) { estado = s; }
    void setRol(const string& r) { rol = r; }

    bool operator==(const Usuario& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Usuario& u) {
        os << "[" << u.id << "] " << left << setw(18) << u.nombre
           << " <" << u.email << "> (" << u.estado << ", " << u.rol << ")";
        return os;
    }
};