#pragma once
#include <iostream>
#include <string>
using namespace std;

class Amistad
{
private:
    int idUsuario1, idUsuario2;

public:
    Amistad(int usuario1 = 0, int usuario2 = 0) {
        idUsuario1 = usuario1;
        idUsuario2 = usuario2;
    }

    // Getters
    int getIdUsuario1() const { return idUsuario1; }
    int getIdUsuario2() const { return idUsuario2; }

    bool operator==(const Amistad& otra) const {
        return (idUsuario1 == otra.idUsuario1 && idUsuario2 == otra.idUsuario2) ||
               (idUsuario1 == otra.idUsuario2 && idUsuario2 == otra.idUsuario1);
    }

    friend ostream& operator<<(ostream& os, const Amistad& amistad) {
        os << amistad.idUsuario1 << " <-> " << amistad.idUsuario2;
        return os;
    }
};