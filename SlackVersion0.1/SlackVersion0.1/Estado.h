#pragma once
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Estado
{
private:
    int id, idUsuario;
    string texto, emoji;

public:
    Estado(int i = 0, int usuario = 0, string t = "", string e = "") {
        id = i;
        idUsuario = usuario;
        texto = t;
        emoji = e;
    }

    // Getters
    int getId() const { return id; }

    bool operator==(const Estado& o) const { return id == o.id; }

    friend ostream& operator<<(ostream& os, const Estado& e) {
        os << "Estado#" << e.id << " User:" << e.idUsuario << " " << e.emoji << " " << e.texto;
        return os;
    }
};