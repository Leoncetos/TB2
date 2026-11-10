#pragma once
#include "ContenidoMensaje.h"
using namespace std;

class Emoji : public ContenidoMensaje
{
private:
    string nombre;

public:
    Emoji(string n = "") { nombre = n; }
    string generarContenido() const override { return ":" + nombre + ":"; }
    string getTipo() const override { return "emoji"; }
};