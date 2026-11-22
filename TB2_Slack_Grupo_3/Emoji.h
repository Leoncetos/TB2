#pragma once
#include <string>
#include "ContenidoMensaje.h"

using namespace std;

class Emoji : public ContenidoMensaje {
private:
    string nombre;

public:
    Emoji(string nom = "") { nombre = nom; }
    string generarContenido() const override { return ":" + nombre + ":"; }
    string getTipo() const override { return "emoji"; }
};